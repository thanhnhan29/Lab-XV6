#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgpte(void)
{
  uint64 va;
  struct proc *p;  

  p = myproc();
  argaddr(0, &va);
  pte_t *pte = pgpte(p->pagetable, va);
  if(pte != 0) {
      return (uint64) *pte;
  }
  return 0;
}
#endif

#ifdef LAB_PGTBL
int
sys_kpgtbl(void)
{
  struct proc *p;  

  p = myproc();
  vmprint(p->pagetable);
  return 0;
}
#endif


uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_pgaccess(void)
{
  uint64 base;      // Địa chỉ ảo bắt đầu
  int len;          // Số lượng trang cần kiểm tra
  uint64 mask_addr; // Địa chỉ buffer user để lưu kết quả bitmask
  uint64 bitmask = 0; // Biến tạm lưu kết quả trong kernel [cite: 203]

  // 1. Lấy tham số từ user [cite: 202]
  argaddr(0, &base);
  argint(1, &len);
  argaddr(2, &mask_addr);

  // Giới hạn len để tránh tràn bitmask (tối đa 64 trang vì uint64 có 64 bit) [cite: 205]
  if(len > 64)
    len = 64;

  struct proc *p = myproc();

  // 2. Duyệt qua từng trang
  for(int i = 0; i < len; i++){
    uint64 va = base + i * PGSIZE; // Tính địa chỉ ảo của trang thứ i
    
    // Dùng hàm walk để tìm PTE tương ứng với va
    pte_t *pte = walk(p->pagetable, va, 0);

    // Kiểm tra xem PTE có tồn tại, có Valid và CÓ BIT ACCESSED không?
    if(pte != 0 && (*pte & PTE_V) && (*pte & PTE_A)){
      // Nếu có truy cập: Bật bit thứ i trong bitmask kết quả
      bitmask |= (1L << i);

      // QUAN TRỌNG: Xóa bit A sau khi kiểm tra 
      // Để lần sau gọi pgaccess, nếu user không đụng vào trang này nữa thì bit A sẽ bằng 0
      *pte &= ~PTE_A; 
    }
  }

  // 3. Copy bitmask kết quả từ kernel ra user space [cite: 204]
  if(copyout(p->pagetable, mask_addr, (char *)&bitmask, sizeof(bitmask)) < 0)
    return -1;

  return 0;
}