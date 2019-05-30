#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_FILE_NAME_SIZE 100

//U:\subjects\C\assignment\part3\tiny8_4
struct Pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct RGB_Image
{
    char file_name[MAX_FILE_NAME_SIZE];
    int height;
    int width;
    int size;
    struct Pixel** pixels;
};
void change_luminosity_image();

//FILE FUNCTIONS AND DYNAMIC MEMORY ALLOCATION
int load_image(struct RGB_Image* image_ptr);
int save_image(struct RGB_Image image);

//FREE FUNCTION
void free_pixels(struct RGB_Image image);

//REALLOC FUNCTION

//IMAGE FUNCTION
void print_information_image();
void save_copy_image();
 
int main(void)
{
    // printf("**19539061") TODO: print student id and assignment3
    //Test functions
    // struct RGB_Image image;
    // print_information_image();
    //  load_image(&image);
    // save_image(image);
    save_copy_image();




    // int choice;
    //  printf("vWu8\bjG\n");
    // do{
       
    //     printf("IMAGE MENU\n");
    //     printf("0. Print Image Information\n");
    //     printf("1. Save Copy of Image\n");
    //     printf("2. Change Luminosity\n");
    //     printf("3. Remove Image Channel\n");
    //     printf("4. Invert Image Colours\n");
    //     printf("5. Quantize Image\n");
    //     printf("6. Flip Image Horizontally\n");
    //     printf("7. Crop Image\n");
    //     printf("-1. Quit\nChoice>>");
    //     scanf("%d", &choice);

        
    //     switch (choice)
    //     {
    //         case 0:
    //             /* code */
    //             printf("%d\n",choice);
    //             print_information_image();
    //             break;
    //         case 1:
    //         printf("%d\n",choice);
    //            // save_image()
    //             /* code */
    //             break;
    //         case 2:
    //         printf("%d\n",choice);
    //             /* code */
    //             break;
    //         case 3:
    //         printf("%d\n",choice);
    //             /* code */
    //             break;
    //         case 4:
    //         printf("%d\n",choice);
    //             /* code */
    //             break;
    //         case 5:
    //         printf("%d\n",choice);
    //             /* code */
    //             break;
    //         case 6:
    //         printf("%d\n",choice);
    //             /* code */
    //             break;
    //         case 7:
    //         printf("%d\n",choice);
    //             /* code */
    //             break;
        
    //         default:
    //             break;
    //     }
    // }while(choice != -1);

    return 0;
}

int load_image(struct RGB_Image* image_ptr)
{

    printf("Enter name of the image:");
    scanf("%s", &image_ptr->file_name);
    printf("%s\n",image_ptr->file_name);
    char file_name[MAX_FILE_NAME_SIZE+4];
    //generate file name
    strcpy(file_name, image_ptr->file_name); 
    strcat(file_name,".bmp" );
    puts(file_name);
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL)
    {
        printf("File can not be opened\n");
        return 1;
    }
    fseek(fp, 2, SEEK_CUR);
    fread(&(image_ptr->size),4,1,fp);
    fseek(fp, 12, SEEK_CUR);
    fread(&(image_ptr->width),4,1,fp);//get width of the image
    fread(&(image_ptr->height),4,1,fp);//get height of the image
    fseek(fp, 28, SEEK_CUR);
    //allocate memory for the pixels
    image_ptr->pixels = (struct Pixel**)malloc(sizeof(struct Pixel*)*image_ptr->height);
    int i,j;
    //loop over each pixel
    for(i = 0 ; i < image_ptr -> height;i++)
    {
       image_ptr->pixels[i] =  (struct Pixel*)malloc(sizeof(struct Pixel) * image_ptr->width);
    }

    for(i = 0 ; i < image_ptr->height;i++)
    {
        for(j =0;j < image_ptr->width;j++)
        {
            fread(&(image_ptr->pixels[i][j].blue),1,1,fp);
            fread(&(image_ptr->pixels[i][j].green),1,1,fp);
            fread(&(image_ptr->pixels[i][j].red),1,1,fp);
        }
    }
    	
    fclose(fp);
    printf("Image Loaded\n\n");
    return 0;

}


void print_information_image()
{
    struct RGB_Image image;
    if(load_image(&image) == 1)
    {
        printf("Image not loaded.\n");
        return;
    }
    printf("File name: %s\n", image.file_name);
    printf("Height:%d\n",image.height);
    printf("Width: %d\n", image.width);
    printf("File size: %d\n", image.size);
    free_pixels(image);

}

int save_image(struct RGB_Image image){
	char file_name[MAX_FILE_NAME_SIZE+4];
	strcpy( file_name, image.file_name);
	strcat(file_name, ".bmp");
	FILE *fp = fopen(file_name, "w");
	if(fp == NULL)
	{
		printf("File can not be saved");
		return 1;
	}
	
	int image_datasize = image.size - 40;
	unsigned char bmp_header [] = {
		0x42, 0x4D,
		image.size, image.size>>8, image.size>>16, image.size>>24,
		0x00, 0x00, 0x00, 0x00,
		0x36, 0x00, 0x00, 0x00,
		0x28, 0x00, 0x00, 0x00,
		image.width, image.width>>8, image.width>>16, image.width>>24,
		image.height, image.height>>8, image.height>>16, image.height >>24,
		0x01, 0x00, 0x18, 0x00,
		0x00,0x00,0x00,0x00,
		image_datasize, image_datasize>>8, image_datasize>>16, image_datasize>>24,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00,
	};
	fwrite(bmp_header, 1, 54, fp);
    int i,j;
    for(i = 0 ; i < image.height; i++)
    {
        for(j=0; j < image.width;j++)
        {
             fwrite(&(image.pixels[i][j].blue) , 1 , 1 , fp );
             fwrite(&(image.pixels[i][j].green) , 1 , 1 , fp );
             fwrite(&(image.pixels[i][j].red) , 1 , 1 , fp );
        }
    }

    fclose(fp);
    printf("Image Saved\n");
    return 0;
	
	
}

void save_copy_image()
{
    struct RGB_Image image;
    if(load_image(&image) == 0)
    {
        strcat(image.file_name,"_copy");
        printf("Image Copied\n\n");
        save_image(image);
        free_pixels(image);
    }
}

void free_pixels(struct RGB_Image image)
{
    int i;
    for(i = 0 ; i < image.height;i++)
    {
        free(image.pixels[i]);
    }
    free(image.pixels);
}

void change_luminosity_image()
{
    struct RGB_Image image;
}
