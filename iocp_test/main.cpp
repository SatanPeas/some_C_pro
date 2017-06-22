#include "test_iocp.h"
#include <iostream>
int main()
{
	CIOCPModel *ciocpModel = new CIOCPModel();
	ciocpModel->LoadSocketLib();
	if (!ciocpModel->Start())
	{
		return 1;
	}
	printf("threads num: %d", ciocpModel->m_nThreads);
	WaitForMultipleObjects(ciocpModel->m_nThreads, ciocpModel->m_phWorkerThreads, TRUE, INFINITE);
	return 0;
}