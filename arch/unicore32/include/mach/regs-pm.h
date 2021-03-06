/*
                                       
 */
/*
                         
 */
#define PM_PMCR                 (PKUNITY_PM_BASE + 0x0000)
/*
                               
 */
#define PM_PGCR                 (PKUNITY_PM_BASE + 0x0004)
/*
                           
 */
#define PM_PPCR                 (PKUNITY_PM_BASE + 0x0008)
/*
                               
 */
#define PM_PWER                 (PKUNITY_PM_BASE + 0x000C)
/*
                                   
 */
#define PM_PGSR                 (PKUNITY_PM_BASE + 0x0010)
/*
                            
 */
#define PM_PCGR                 (PKUNITY_PM_BASE + 0x0014)
/*
                                    
 */
#define PM_PLLSYSCFG            (PKUNITY_PM_BASE + 0x0018)
/*
                                    
 */
#define PM_PLLDDRCFG            (PKUNITY_PM_BASE + 0x001C)
/*
                                    
 */
#define PM_PLLVGACFG            (PKUNITY_PM_BASE + 0x0020)
/*
                             
 */
#define PM_DIVCFG               (PKUNITY_PM_BASE + 0x0024)
/*
                                        
 */
#define PM_PLLSYSSTATUS         (PKUNITY_PM_BASE + 0x0028)
/*
                                        
 */
#define PM_PLLDDRSTATUS         (PKUNITY_PM_BASE + 0x002C)
/*
                                        
 */
#define PM_PLLVGASTATUS         (PKUNITY_PM_BASE + 0x0030)
/*
                                 
 */
#define PM_DIVSTATUS            (PKUNITY_PM_BASE + 0x0034)
/*
                                   
 */
#define PM_SWRESET              (PKUNITY_PM_BASE + 0x0038)
/*
                                     
 */
#define PM_DDR2START            (PKUNITY_PM_BASE + 0x003C)
/*
                                     
 */
#define PM_DDR2CAL0             (PKUNITY_PM_BASE + 0x0040)
/*
                                    
 */
#define PM_PLLDFCDONE           (PKUNITY_PM_BASE + 0x0044)

#define PM_PMCR_SFB             FIELD(1, 1, 0)
#define PM_PMCR_IFB             FIELD(1, 1, 1)
#define PM_PMCR_CFBSYS          FIELD(1, 1, 2)
#define PM_PMCR_CFBDDR          FIELD(1, 1, 3)
#define PM_PMCR_CFBVGA          FIELD(1, 1, 4)
#define PM_PMCR_CFBDIVBCLK      FIELD(1, 1, 5)

/*
                                            
 */
#define PM_PWER_GPIOHIGH        FIELD(1, 1, 8)
/*
                                       
 */
#define PM_PWER_RTC             FIELD(1, 1, 31)

#define PM_PCGR_BCLK64DDR	FIELD(1, 1, 0)
#define PM_PCGR_BCLK64VGA	FIELD(1, 1, 1)
#define PM_PCGR_BCLKDDR		FIELD(1, 1, 2)
#define PM_PCGR_BCLKPCI		FIELD(1, 1, 4)
#define PM_PCGR_BCLKDMAC	FIELD(1, 1, 5)
#define PM_PCGR_BCLKUMAL	FIELD(1, 1, 6)
#define PM_PCGR_BCLKUSB		FIELD(1, 1, 7)
#define PM_PCGR_BCLKMME		FIELD(1, 1, 10)
#define PM_PCGR_BCLKNAND	FIELD(1, 1, 11)
#define PM_PCGR_BCLKH264E	FIELD(1, 1, 12)
#define PM_PCGR_BCLKVGA		FIELD(1, 1, 13)
#define PM_PCGR_BCLKH264D	FIELD(1, 1, 14)
#define PM_PCGR_VECLK		FIELD(1, 1, 15)
#define PM_PCGR_HECLK		FIELD(1, 1, 16)
#define PM_PCGR_HDCLK		FIELD(1, 1, 17)
#define PM_PCGR_NANDCLK		FIELD(1, 1, 18)
#define PM_PCGR_GECLK		FIELD(1, 1, 19)
#define PM_PCGR_VGACLK          FIELD(1, 1, 20)
#define PM_PCGR_PCICLK		FIELD(1, 1, 21)
#define PM_PCGR_SATACLK		FIELD(1, 1, 25)

/*
                             
 */
#define PM_DIVCFG_VGACLK_MASK   FMASK(4, 20)
#define PM_DIVCFG_VGACLK(v)	FIELD((v), 4, 20)

#define PM_SWRESET_USB          FIELD(1, 1, 6)
#define PM_SWRESET_VGADIV       FIELD(1, 1, 26)
#define PM_SWRESET_GEDIV        FIELD(1, 1, 27)

#define PM_PLLDFCDONE_SYSDFC    FIELD(1, 1, 0)
#define PM_PLLDFCDONE_DDRDFC    FIELD(1, 1, 1)
#define PM_PLLDFCDONE_VGADFC    FIELD(1, 1, 2)
