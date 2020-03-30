// һЩ���õġ����õĽӿ�

/*
 * ��ָ���Ĵ�����ָ��λ��1
 * @reg �Ĵ�����ַ
 * @bit ��Ҫ��1����һbit
 */
void reg_set_one_bit(volatile unsigned int *reg, unsigned int bit)
{
    unsigned int temp, mask;

    mask = 1 << bit;
    temp = *reg;
    temp |= mask;
    *reg = temp;

    return ;
}


/*
 * ��ָ���Ĵ�����ָ��λ����
 * @reg �Ĵ�����ַ
 * @bit ��Ҫ�������һbit
 */
void reg_clr_one_bit(volatile unsigned int *reg, unsigned int bit)
{
    unsigned int temp, mask;

    mask = 1 << bit;
    temp = *reg;
    temp &= ~mask;
    *reg = temp;

    return ;
}



/*
 * ��ȡָ���Ĵ�����ָ��λ��ֵ
 * @reg �Ĵ�����ַ
 * @bit ��Ҫ��ȡֵ����һbit
 * @ret ָ��λ��ֵ
 */
unsigned int reg_get_bit(volatile unsigned int *reg, unsigned int bit)
{
    unsigned int temp;

    temp = *reg;
    temp = (temp >> bit) & 1;

    return temp;
}


/*
 * ��Ĵ�����д��8bit(һ���ֽ�)����
 * @data ��д�������
 * @addr �Ĵ�����ַ
 */
void reg_write_8(unsigned char data, volatile unsigned char *addr)
{
    *addr = data;
}


/*
 * �ӼĴ�������8bit(һ���ֽ�)����
 * @addr �Ĵ�����ַ
 * @ret ����������
 */
unsigned char reg_read_8(volatile unsigned char *addr)
{
    return (*addr);
}


/*
 * ��Ĵ�����дһ��32bit������
 * @data ��д�������
 * @addr �Ĵ�����ַ
 */
void reg_write_32(unsigned int data, volatile unsigned int *addr)
{
    *addr = data;
}


/*
 * �ӼĴ�������һ��32bit����
 * @addr �Ĵ�����ַ
 * @ret ����������
 */
unsigned int reg_read_32(volatile unsigned int *addr)
{
    return (*addr);
}



/**
 * ffs - find first bit set
 * @x: the word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */
int ls1b_ffs(int x)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}


