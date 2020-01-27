#ifndef __SUPER__
#define __SUPER__

typedef enum { diary, meat, neutral, fruit, vegetable, nofTypes } ProductType;

#define FILE_NAME "ProdList.bin" 

#define MAX_VOLUME 50

typedef struct
{
	int		num;
	ProductType type;
	float	volume;
	float	price;
}Product;


typedef struct
{
	ProductType type;
	float	volume;
	float	maxVolume;
	int		productCount;
	Product**	prodArr;
}Bag;

void Q1_Super();
Product** createProductArrFromFile(int* numOfProducts);
Product* createProductFromFile(FILE* file);
Bag** packToBags(Product** allProducts, const int numOfProducts, int* numOfBags);
Bag* createNewBag(ProductType type);
int productToBag(Bag* bag, Product* product);
int findBag(Bag** allBags, const int numOfBags, Product* product);
void printProduct(const Product* pProduct);
void printBeg(const Bag* pBeg);
void printBegArr(const Bag** pBegArr, int count);
void releaseAll(Bag** allBags, int numOfBags);

#endif