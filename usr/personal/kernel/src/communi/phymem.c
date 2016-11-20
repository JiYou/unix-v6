/* 
 * 分配blocks个连续物理内存块，返回地址。其起始地址必须是内存块大小的整数倍，
* 也就是从某个物理内存块开始。
*/
char *phymalloc(int blocks)
{
	register int a, b, c;
	int  addr, bytes=blocks*PHY_MEM_BLOCK_SIZE;
	register struct map *bp, bp2;

  for (bp = coremap; bp->m_size; bp++) 
	{
		addr = (bp->m_addr+PHY_MEM_BLOCK_SIZE-1)& 
										PHY_MEM_BLOCK_SIZE;
		if ((c=bp->m_addr+bp->m_size –addr)>= bytes) 
		{
				a = addr;
				c -= bytes;
				if((b = addr - bp->m_addr) > 0)
				{
					bp->m_size = b;
					if(c == 0)
					{
						return a;
					}
					while((++bp2)->m_size);
					while(bp2-1>bp)
					{
						bp2->m_size = (bp2-1)->m_size;
						bp2->m_addr = (--bp2)->m_addr;
					}
					bp2->m_addr = bp->addr+b+bytes;
					bp2->m_size = c;
					return a;
				}
				else if(c == 0)
				{
					do { 
						bp++;
						(bp-1)->m_addr = bp->m_addr;
					} while((bp-1)->m_size = bp->m_size)
					return(a);
				}
			
				bp->m_addr+=bytes;
				bp->m_size = c;
				return a;
		}
  }
  return(0);
}

void phyfree(char *addr, int blocks)
{
	mfree(coremap, blocks* PHY_MEM_BLOCK_SIZE , addr);
}