/*************************************************************************
 *
 * ���ٻ������ͷ�ļ�
 *
 *************************************************************************/



// һ��cache��������Ϣ
struct cache_desc {
    unsigned int waysize;           // Bytes per way ÿ·���ֽ���
    unsigned short sets;            // Number of lines per set ��Ӧ�Ĵ����е�S������
    unsigned char ways;             // Number of ways ��Ӧ�Ĵ����е�A��������·��
    unsigned char linesz;           // size of line in bytes ��Ӧ�Ĵ����е�L���д�С
    unsigned char waybit;           // Bits to select in a cache set 

    unsigned long cachesize;        // ���ٻ����С
};



// ̽��cache���ͺʹ�С
void cache_probe(void);

// ��ʼ�����ٻ���
void cache_init(void);




// ��дdcache��ĳ��ָ������
// @start ����д����ʼ��ַ
// @end ����д�Ľ�����ַ
void dcache_writeback_invalidate_range(unsigned long start, unsigned long end);

// ����icache��ĳ��ָ������
// @start �����ϵ���ʼ��ַ
// @end �����ϵĽ�����ַ
void icache_invalidate_range(unsigned long start, unsigned long end);


// ��д����dcache
void dcache_writeback_invalidate_all(void);

// ��������icache
void icache_invalidate_all(void);





