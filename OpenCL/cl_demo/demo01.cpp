#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <iostream>  

#ifdef MAC  
#include <OpenCL/cl.h>  
#else  
#include <CL/cl.h>  
#endif  

int main() {  

	/* Host data structures */  
	cl_platform_id *platforms;  
	//ÿһ��cl_platform_id �ṹ��ʾһ���������ϵ�OpenCLִ��ƽ̨������ָ������֧��OpenCL��Ӳ������nvidia�Կ���intel CPU���Կ���AMD�Կ���CPU��  
	cl_uint num_platforms;  
	cl_int err, platform_index = -1;  

	/* Extension data */  
	char* ext_data;                           
	size_t ext_size;     
	const char icd_ext[] = "cl_khr_icd";  

	cl_uint numDevices = 0;
	cl_device_id *devices = NULL;

	char        *value;
	size_t      valueSize;
	size_t      maxWorkItemPerGroup;
	cl_uint     maxComputeUnits=0;
	cl_ulong    maxGlobalMemSize=0;
	cl_ulong    maxConstantBufferSize=0;
	cl_ulong    maxLocalMemSize=0;

	//Ҫʹplatform��������Ҫ�������衣1 ��ҪΪcl_platform_id�ṹ�����ڴ�ռ䡣2 ��Ҫ����clGetPlatformIDs��ʼ����Щ���ݽṹ��һ�㻹��Ҫ����0��ѯ���������ж���platforms  

	/* Find number of platforms */  
	//����ֵ���Ϊ-1��˵�����ú���ʧ�ܣ����Ϊ0�����ɹ�  
	//�ڶ�������ΪNULL����Ҫ��ѯ�������ж��ٸ�platform����ʹ��num_platformsȡ��ʵ��flatform������  
	//��һ������Ϊ1������������Ҫȡ���1��platform�����Ը�Ϊ������磺INT_MAX�������ֵ�����Ǿ�˵0������ᱨ��ʵ�ʲ��Ժ��񲻻ᱨ�������ǲ���0��ѯ�������ж���platforms  
	err = clGetPlatformIDs(5, NULL, &num_platforms);          
	if(err < 0) {          
		perror("Couldn't find any platforms.");           
		exit(1);                              
	}                                     

	printf("I have platforms: %d\n", num_platforms); //���˼��������ʾΪ2����intel��nvidia����ƽ̨  

	/* Access all installed platforms */  
	//����1 ����cl_platform_id��������ռ�  
	platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * num_platforms);   
	//����2 �ڶ���������ָ��platforms�洢platform  
	clGetPlatformIDs(num_platforms, platforms, NULL);         

	/* Find extensions of all platforms */  
	//��ȡ�����ƽ̨��Ϣ�������Ѿ�ȡ����ƽ̨id�ˣ���ô�Ϳ��Խ�һ����ȡ������ϸ����Ϣ�ˡ�  
	//һ��forѭ����ȡ���е������ϵ�platforms��Ϣ  
	for(cl_int i=0; i<num_platforms; i++)   
	{  
		/* Find size of extension data */  
		//Ҳ�Ǻ�ǰ��һ���������õ����͵��ĸ�����Ϊ0��NULL��Ȼ��Ϳ����õ��������ext_size��ȡ������Ϣ�ĳ����ˡ�  
		err = clGetPlatformInfo(platforms[i],             
			CL_PLATFORM_EXTENSIONS, 0, NULL, &ext_size);      
		if(err < 0)   
		{  
			perror("Couldn't read extension data.");              
			exit(1);  
		}     

		printf("The size of extension data is: %d\n", ext_size);//�ҵļ������ʾ224.  

		/* Access extension data */    
		//�����ext_data�൱��һ�����棬�洢�����Ϣ��  
		ext_data = (char*)malloc(ext_size);   
		//����������ǻ�ȡ�����Ϣ�ĺ������ڶ�������ָ������Ҫʲô������Ϣ���������CL_PLATFORM_EXTENSIONS��ʾ��opencl֧�ֵ���չ������Ϣ���Ҽ�������һ�󴮣������Ƚ��£�ר��Ϊ��ѧͼ��ѧ�����õĵ��ԣ���֧�ֵĶ����Ƚ϶ࡣ  
		clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS,       
			ext_size, ext_data, NULL);                
		printf("Platform %d supports extensions: %s\n", i, ext_data);  

		//��������������̵����֣��������Կ���Ϣ�ǣ�NVIDIA CUDA  
		char *name = (char*)malloc(ext_size);  
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME,     
			ext_size, name, NULL);                
		printf("Platform %d name: %s\n", i, name);  

		//�����ǹ�Ӧ����Ϣ�����Կ���Ϣ��NVIDIA Corporation  
		char *vendor = (char*)malloc(ext_size);  
		clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR,       
			ext_size, vendor, NULL);                  
		printf("Platform %d vendor: %s\n", i, vendor);  

		//���֧�ֵ�OpenCL�汾��������ʾ��OpenCL1.1 CUDA 4.2.1  
		char *version = (char*)malloc(ext_size);  
		clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION,      
			ext_size, version, NULL);                 
		printf("Platform %d version: %s\n", i, version);  

		//���ֻ������ֵ��full profile �� embeded profile  
		char *profile = (char*)malloc(ext_size);  
		clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE,      
			ext_size, profile, NULL);                 
		printf("Platform %d full profile or embeded profile?: %s\n", i, profile);  

		/* Look for ICD extension */     
		//���֧��ICD��һ��չ���ܵ�platform�������ʾ��������Intel��Nvidia��֧����һ��չ����  
		if(strstr(ext_data, icd_ext) != NULL)   
			platform_index = i;  
		std::cout<<"Platform_index = "<<platform_index<<std::endl;  
		/* Display whether ICD extension is supported */  
		if(platform_index > -1)  
			printf("Platform %d supports the %s extension.\n",   
			platform_index, icd_ext);  

		//�ͷſռ�  
		free(ext_data);  
		free(name);  
		free(vendor);  
		free(version);  
		free(profile);  


		//-----------------------------------------------------
		// Discover and initialize the devices
		//----------------------------------------------------- 

		// Use clGetDeviceIDs() to retrieve the number of 
		// devices present
		clGetDeviceIDs(
			platforms[i], 
			CL_DEVICE_TYPE_ALL, 
			0, 
			NULL, 
			&numDevices);

		// Allocate enough space for each device
		devices = (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));

		// Fill in devices with clGetDeviceIDs()
		clGetDeviceIDs(
			platforms[i], 
			CL_DEVICE_TYPE_ALL,        
			numDevices, 
			devices, 
			NULL);

		for (cl_int j=0;j<numDevices;j++)
		{
			//print the device name
			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &valueSize);
			value = (char*) malloc(valueSize);
			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, NULL);
			printf("Platform %d, Device %d, Device Name: %s\n", i,j,value);
			free(value);

			/// print parallel compute units(CU)
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(maxComputeUnits), &maxComputeUnits, NULL);
			printf("Platform %d, Device %d, Parallel compute units: %u\n", i,j,maxComputeUnits);
			 
			///maxWorkItemPerGroup
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(maxWorkItemPerGroup), &maxWorkItemPerGroup, NULL);
			printf("Platform %d, Device %d, maxWorkItemPerGroup: %zd\n", i,j,maxWorkItemPerGroup);
			 
			/// print maxGlobalMemSize
			clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE,sizeof(maxGlobalMemSize), &maxGlobalMemSize, NULL);
			printf("Platform %d, Device %d, maxGlobalMemSize: %lu(MB)\n", i,j,maxGlobalMemSize/1024/1024);
			
			/// print maxConstantBufferSize
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,sizeof(maxConstantBufferSize), &maxConstantBufferSize, NULL);
			printf("Platform %d, Device %d, maxConstantBufferSize: %lu(KB)\n", i,j,maxConstantBufferSize/1024);
			
			/// print maxLocalMemSize
			clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE,sizeof(maxLocalMemSize), &maxLocalMemSize, NULL);
			printf("Platform %d, Device %d, maxLocalMemSize: %lu(KB)\n", i,j,maxLocalMemSize/1024);
		}

		free(devices);

		std::cout<<std::endl;
		std::cout<<std::endl;
	}  

	if(platform_index <= -1)  
		printf("No platforms support the %s extension.\n", icd_ext);  

	/* Deallocate resources */  
	free(platforms); 

	return 0;  
} 
