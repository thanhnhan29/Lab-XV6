	.text

	/*
         * save the old thread's registers,
         * restore the new thread's registers.
         */

	.globl thread_switch
thread_switch:
	/* LƯU TRẠNG THÁI CŨ VÀO a0 */
    sd ra, 0(a0)    /* Lưu thanh ghi ra vào vị trí offset 0 của a0 */
    sd sp, 8(a0)    /* Lưu thanh ghi sp vào vị trí offset 8 (0+8 byte) */
    sd s0, 16(a0)
    sd s1, 24(a0)
    sd s2, 32(a0)
    sd s3, 40(a0)
    sd s4, 48(a0)
    sd s5, 56(a0)
    sd s6, 64(a0)
    sd s7, 72(a0)
    sd s8, 80(a0)
    sd s9, 88(a0)
    sd s10, 96(a0)
    sd s11, 104(a0)

	/* KHÔI PHỤC TRẠNG THÁI MỚI TỪ a1 */
    ld ra, 0(a1)    /* Lấy giá trị tại a1 nạp lại vào ra */
    ld sp, 8(a1)
    ld s0, 16(a1)
    ld s1, 24(a1)
    ld s2, 32(a1)
    ld s3, 40(a1)
    ld s4, 48(a1)
    ld s5, 56(a1)
    ld s6, 64(a1)
    ld s7, 72(a1)
    ld s8, 80(a1)
    ld s9, 88(a1)
    ld s10, 96(a1)
    ld s11, 104(a1)

	ret    /* Trở về địa chỉ ra vừa được khôi phục -> Thread mới bắt đầu chạy */

