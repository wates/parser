//
//#include <vector>
//
//struct Runner
//{
//	virtual void Run()=0;
//};
//
//std::vector<Runner*> runs;
//
//template<typename T>
//struct Tester
//{
//	Tester()
//	{
//		runs.push_back(this);
//	}
//	void Run()
//	{
//		t=new T;
//	}
//};
//
//struct TesterExe
//{
//	TesterExe()
//	{
//	}
//};
//
//
//void Run()
//{
//	for(int i=0;i<runs.size();i++)
//		runs[i]->Run();
//}
//
//
