#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc ,char *argv[])
{
    FILE *infile, *outfile;	
    char infname[1024];
    char outfname[1024];
    char fw_name[128];
    char *rt28xxdir;
    char *chipset;
	char *wow, *rt28xx_mode; /* for WOW firmware */
    int i=0;//,n=0;
    unsigned char c;
   
    memset(infname,0,1024);
    memset(outfname,0,1024);
    memset(fw_name, 0, 128);
    
    rt28xxdir = (char *)getenv("RT28xx_DIR");
    chipset = (char *)getenv("CHIPSET");
	wow = (char *)getenv("HAS_WOW_SUPPORT"); /* for WOW firmware */
	rt28xx_mode = (char *)getenv("RT28xx_MODE");

    if(!rt28xxdir) {
         printf("Environment value \"RT28xx_DIR\" not export \n");
	 return -1;
    }

    if(!chipset) {
	 printf("Environment value \"CHIPSET\" not export \n");
	 return -1;
    }	    
	
    if (strlen(rt28xxdir) > (sizeof(infname)-100)) {
	 printf("Environment value \"RT28xx_DIR\" is too long!\n");
	 return -1;
    }

    strcat(infname,rt28xxdir);
    strcat(outfname,rt28xxdir);
    
    if (strncmp(chipset, "2860",4) == 0) {
        strcat(infname,"/mcu/bin/rt2860.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "2870",4) == 0) {
        strcat(infname,"/mcu/bin/rt2870.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "3090",4) == 0) {
        strcat(infname,"/mcu/bin/rt2860.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "2070",4) == 0) {
        strcat(infname,"/mcu/bin/rt2870.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "3070",4) == 0) {
        strcat(infname,"/mcu/bin/rt2870.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "3572",4) == 0) {
        strcat(infname,"/mcu/bin/rt2870.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "3573",4) == 0) {
        strcat(infname,"/mcu/bin/rt2870.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "3370",4) == 0) {
        strcat(infname,"/mcu/bin/rt2870.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "5370",4) == 0) {
        strcat(infname,"/mcu/bin/rt2870.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "5572",4) == 0) {
        strcat(infname,"/mcu/bin/rt2870.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "5592",4) == 0) {
        strcat(infname,"/mcu/bin/rt2860.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "USB",3) == 0) {
        strcat(infname,"/mcu/bin/rt2870.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if (strncmp(chipset, "PCI",3) == 0) {
        strcat(infname,"/mcu/bin/rt2860.bin");
        strcat(outfname,"/include/mcu/firmware.h");
        strcat(fw_name, "FirmwareImage");
    } else if ((strncmp(chipset, "6590", 4) == 0)
            || (strncmp(chipset, "6570", 4) == 0)) {
        strcat(infname,"/mcu/bin/MT7650.bin");
        strcat(outfname,"/include/mcu/MT7650_firmware.h");
        strcat(fw_name, "MT7650_FirmwareImage");
    } else if ((strncmp(chipset, "MT7662E", 7) == 0)
            || (strncmp(chipset, "MT7662U", 7) == 0)) {
        strcat(infname,"/mcu/bin/MT7662.bin");
        strcat(outfname,"/include/mcu/MT7662_firmware.h");
        strcat(fw_name, "MT7662_FirmwareImage");
    } else if(strncmp(chipset, "7601",4)==0) {
        strcat(infname,"/mcu/bin/MT7601.bin");
        strcat(outfname,"/include/mcu/MT7601_firmware.h");
        strcat(fw_name, "MT7601_FirmwareImage");
    } else {
        strcat(infname,"/mcu/bin/rt2860.bin");
        strcat(outfname,"/include/firmware.h");
    }

	/* for WOW support firmware */
	if ((wow != NULL) && (strncmp(wow, "y", 1) == 0) && (strncmp(rt28xx_mode, "STA", 3) == 0))
	{
		if ((wow = strstr(infname, "rt2870")) != NULL)
		{
			strcpy(wow, "rt2870_wow.bin");	
			fprintf(stderr, "infname %s\n", infname);
		}
	}
	    
    infile = fopen(infname,"r");
     
    if (infile == (FILE *) NULL)
    {
         printf("Can't read file %s \n",infname);
	 return -1;
    }

    outfile = fopen(outfname,"w");
    
    if (outfile == (FILE *) NULL)
    {
         printf("Can't open write file %s \n",outfname);
        return -1;
    }
    
    fputs("/* AUTO GEN PLEASE DO NOT MODIFY IT */ \n",outfile);
    fputs("/* AUTO GEN PLEASE DO NOT MODIFY IT */ \n",outfile);
    fputs("\n",outfile);
    fputs("\n",outfile);

	fprintf(outfile, "UCHAR %s[] = {\n", fw_name);

    while(1)
    {
	char cc[3];    

	c = getc(infile);
	
	if (feof(infile))
	    break;
	
	memset(cc,0,2);
	
	if (i>=16)
	{	
	    fputs("\n", outfile);	
	    i = 0;
	}    
	fputs("0x", outfile); 
	sprintf(cc,"%02x",c);
	fputs(cc, outfile);
	fputs(", ", outfile);
	i++;
    } 
    
    fputs("} ;\n", outfile);
    fclose(infile);
    fclose(outfile);
    exit(0);
}	
