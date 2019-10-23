//Required
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <cmath>

double getHue(double red, double green, double blue)
{
	double rgb[] = { (red/255), (blue/255), (green/255)};
	double * max = std::max_element(rgb, rgb+3);
	double * min = std::min_element(rgb, rgb+3);
	double delta = *max - *min;
	double hue = 0.00;
	if(max == min)
	{
		return 0;
	}
	else if(*max == rgb[0])
	{
		hue = (rgb[1] - rgb[2]) / delta;
	}
	else if(*max == rgb[1])
	{
		hue = 2.00 + (rgb[2] - rgb[0]) / delta;
	}
	else
	{
		hue = 4.00 + (rgb[0] - rgb[1]) / delta;
	}
	hue = hue * 60;
	if (hue < 0)
	{
		hue = hue + 360;
	}
	return hue;
}
double getIntensity(double red, double green, double blue)
{
	double rgb[] = { (red/255), (blue/255), (green/255)};
	double * max = std::max_element(rgb, rgb+3);
	double * min = std::min_element(rgb, rgb+3);
	double delta = *max - *min;
	double L = (rgb[0] + rgb[1] + rgb[2])/3;
	return L;
}
double getSaturation(double red, double green, double blue)
{
	double rgb[] = { (red/255), (blue/255), (green/255)};
	double * max = std::max_element(rgb, rgb+3);
	double * min = std::min_element(rgb, rgb+3);
	double delta = *max - *min;
	double L = (*max + *min)/2;
	if(delta == 0)
	{
		return 0;
	}
	else
	{
		return (double)(delta/(1-(abs(2*L -1))));
	}
	
}
int main(int argc, char* argv[])
{
	//typedef Array <int> Array;
	bool QUIT = false;
	std::string fileName;
	while(!(QUIT))
	{
     
	    std::cout << "REMEMBER TYPE QUIT TO QUIT!!!" << std::endl;
		std::cout << "Please ENTER a filename:" << std::endl;
		std::cout << std::endl;
		std::cout << "~";
		std::getline (std::cin >> std::skipws,fileName);
		std::cout << std::endl;
		if(fileName.compare("QUIT")==0 || fileName.compare("quit")==0)
		{
			QUIT = true;
			break;
		}
		else
		{
            char const* name = fileName.c_str();
			std::cout << name << std::endl;
			FILE * pic = fopen(name, "r");
			FILE * picA = fopen("result(1).raw", "wb+");
			FILE * picB = fopen("result(2).raw", "wb+");
			FILE * picC = fopen("result(3).raw", "wb+");
            if(pic == NULL){
				std::cout << "no file" << std::endl;
				continue;
			}
            fseek(pic, 0, SEEK_END);
            size_t size = ftell(pic);
            rewind(pic);
            unsigned char *buffer = new unsigned char[size];
			unsigned char *dataA = new unsigned char[size];
			unsigned char *dataB = new unsigned char[size];
			unsigned char *dataC = new unsigned char[size];
			size_t len = size/3;
			double hue[len];
			double intensity[len];
			double saturation[len];
			double avgHue = 0;
			double avgInten = 0;
			double avgSat = 0;
			fread(buffer, sizeof(unsigned char), size, pic);
            //move by pixel
			size_t p = 0;//pixel value
			for(size_t i = 0; (i + 2) < size; i += 3)
            {
				int r = (int)buffer[i];
				int g = (int)buffer[i + 1];
				int b  = (int)buffer[i + 2];
				hue[p] = getHue((double)r, (double)g, (double)b);
				intensity[p] = getIntensity((double)r, (double)g, (double)b);
				saturation[p] = getSaturation((double)r, (double)g, (double)b);
				std::cout << p << "| hue: ";
				std::cout << hue[p] << "| intensity: ";
				std::cout << intensity[p] << "| saturation: ";
				std::cout << saturation[p] << std::endl;
				(unsigned char)buffer[i];
				(unsigned char)buffer[i + 1];
				(unsigned char)buffer[i + 2];
				dataA[i]= (buffer[i] * (unsigned char)intensity[p]);
				dataA[i + 1]= (buffer[i + 1] * (unsigned char)intensity[p]);
				dataA[i + 2]= (buffer[i + 2] * (unsigned char)intensity[p]);
				avgHue += hue[p];
				avgInten += intensity[p];
				avgSat += saturation[p];
				p++;

            }
			std::cout << "Length: " <<(double)len << std::endl;
			avgHue = avgHue/(double)len;
			avgInten = avgInten/(double)len;
			avgSat = avgSat/(double)len;
			std::cout << "Average Hue: " << avgHue << std::endl;
			std::cout << "Average Intentsity: " << avgInten << std::endl;
			std::cout << "Average Saturation: " << avgSat << std::endl;

			/**
			//output
			for(size_t i = 0; i < size; i ++)
            {
				printf("%d %n", buffer[i]);
				dataA[i]= abs(buffer[i] - (unsigned char)255);
				dataB[i] = buffer[i] + (unsigned char)20; 
				dataC[i] = buffer[i] + (unsigned char)128; 
            }
			**/
			std::cout << sizeof(buffer) << std::endl;
			std::cout << size << std::endl;
            fwrite(dataA, sizeof(unsigned char), size, picA);
			//fwrite(dataB, sizeof(unsigned char), size, picB);
			//fwrite(dataC, sizeof(unsigned char), size, picC);
            fclose(pic);
			fclose(picA);
			fclose(picB);
			fclose(picC);
			fileName.clear();
			delete [] buffer;
			delete [] dataA;
			delete [] dataB;
			delete [] dataC;
		}
		continue;
	}
	
	return 0;
}