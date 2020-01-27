//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "QSuper.h"

const char TypeStr[nofTypes][15] = { "diary", "meat", "neutral", "fruit", "vegetable" };

void Q1_Super()
{
	Product** allProduct = NULL;
	Bag** allBags = NULL;
	int numOfProducts;
	allProduct = createProductArrFromFile(&numOfProducts);
	int numOfBags;
	allBags = packToBags(allProduct, numOfProducts, &numOfBags);
	printBegArr(allBags, numOfBags);
	releaseAll(allBags, numOfBags);
	//Add here

}

Product* createProductFromFile(FILE* file) {
	Product* product = calloc(1, sizeof(Product));
	if (product == NULL) {
		return NULL;
	}
	fread(&product->num, sizeof(int), 1, file);
	fread(&product->type, sizeof(ProductType), 1, file);
	fread(&product->volume, sizeof(float), 1, file);
	fread(&product->price, sizeof(float), 1, file);
	return product;
}

Product** createProductArrFromFile(int *numOfProducts){
	int i;
	FILE *file = NULL;
	fopen_s(&file, FILE_NAME, "rb");
	if (file == NULL) {
		return NULL;
	}
	if (!fread(numOfProducts, sizeof(int), 1, file)) return NULL;
	Product** allProduct = calloc(*numOfProducts, sizeof(Product*));
	if (allProduct == NULL) {
		return NULL;
	}
	for ( i = 0; i < *numOfProducts; i++)	{
		allProduct[i] = createProductFromFile(file);
	}
	fclose(file);
	return allProduct;
}

Bag** packToBags(Product** allProducts, const int numOfProducts, int* numOfBags) {
	int i, foundBag;
	*numOfBags = 0;
	Bag** allBags = calloc(1, sizeof(Bag*));
	if (allBags == NULL) {
		return NULL;
	}
	for (i = 0; i < numOfProducts; i++) {
		foundBag = findBag(allBags, *numOfBags, allProducts[i]);
		if (foundBag >= 0) {
			if (!productToBag(allBags[foundBag], allProducts[i])) return NULL;
		}
		else {
			int size = *numOfBags + 1;
			Bag** tempArr = allBags;
			allBags = realloc(tempArr, size * sizeof(Bag*));
			if (allBags == NULL) return NULL;
			allBags[*numOfBags] = createNewBag(allProducts[i]->type);
			if (allBags[*numOfBags] == NULL) return NULL;
			if (!productToBag(allBags[*numOfBags], allProducts[i])) return NULL;
			(*numOfBags)++;
		}
	}
	return allBags;
}

Bag* createNewBag(ProductType type) {
	Bag* bag = calloc(1, sizeof(Bag));
	if (bag == NULL) return NULL;
	bag->maxVolume = MAX_VOLUME;
	bag->volume = 0;
	bag->productCount = 0;
	bag->type = type;
	bag->prodArr = calloc(1, sizeof(Product*));
	if (bag->prodArr == NULL) return NULL;
	return bag;
}

int productToBag(Bag* bag, Product* product) {
	if (bag->maxVolume < (bag->volume + product->volume)) return 0;
	int size = bag->productCount + 1;
	Product** tempArr = bag->prodArr;
	bag->prodArr = realloc(tempArr, size * sizeof(Product*));
	if (bag->prodArr == NULL) return 0;
	bag->prodArr[bag->productCount] = product;
	bag->volume += product->volume;
	bag->productCount++;
	return 1;
}

int findBag(Bag** allBags, const int numOfBags, Product* product) {
	int i;
	for ( i = 0; i < numOfBags; i++)	{
		if (product->type == allBags[i]->type && product->volume <= (allBags[i]->maxVolume - allBags[i]->volume)) {
			return i;
		}
	}
	return -1;
}




void releaseAll(Bag** allBags, int numOfBags) {
	int i, j;
	for (i = 0; i < numOfBags; i++) {
		for (j = 0; j < allBags[i]->productCount; j++) {
			free(allBags[i]->prodArr[j]);
		}
		free(allBags[i]->prodArr);
		free(allBags[i]);
	}
	free(allBags);
}


void		printProduct(const Product* pProduct)
{
	printf("product %d type:%s volume: %.2f price: %.2f \n", pProduct->num, TypeStr[pProduct->type],
		pProduct->volume, pProduct->price);
}

void		printBeg(const Bag* pBeg)
{
	int i;
	printf("Beg Type %s\t", TypeStr[pBeg->type]);
	printf("volume %.2f out of %.2f\n", pBeg->volume, pBeg->maxVolume);
	printf("There are %d products in the beg\n", pBeg->productCount);
	for (i = 0; i < pBeg->productCount; i++)
		printProduct(pBeg->prodArr[i]);
}

void	printBegArr(const Bag** pBegArr, int count)
{
	int i;
	printf("There are %d begs in the super\n", count);
	for (i = 0; i < count; i++)
		printBeg(pBegArr[i]);
}


