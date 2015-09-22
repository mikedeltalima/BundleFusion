#pragma once

#include <fstream>
#include <iostream>

class TimingLog
{
public:
	struct FrameTiming {
		double timeSiftDetection; // fuse for global / sift detection for local
		double timeSiftMatching;
		double timeMatchFilterKeyPoint;
		double timeMatchFilterSurfaceArea;
		double timeMatchFilterDenseVerify;
		double timeFilterFrames;
		double timeAddCurrResiduals;
		double timeSolve;
		unsigned int numItersSolve;

		double timeReIntegrate;
		double timeReconstruct;
		double timeVisualize;

		FrameTiming() {
			timeSiftDetection = 0;
			timeSiftMatching = 0;
			timeMatchFilterKeyPoint = 0;
			timeMatchFilterSurfaceArea = 0;
			timeMatchFilterDenseVerify = 0;
			timeFilterFrames = 0;
			timeAddCurrResiduals = 0;
			timeSolve = 0;
			numItersSolve = 0;

			timeReIntegrate = 0;
			timeReconstruct = 0;
			timeVisualize = 0;
		}

		void print(std::ostream* out, bool printDepthSensing) {
			*out << "\tTime SIFT Detection: " << std::to_string(timeSiftDetection) << "ms" << std::endl;
			*out << "\tTime SIFT Matching: " << std::to_string(timeSiftMatching) << "ms" << std::endl;
			*out << "\tTime Match Filter Key Point: " << std::to_string(timeMatchFilterKeyPoint) << "ms" << std::endl;
			*out << "\tTime Match Filter Surface Area: " << std::to_string(timeMatchFilterSurfaceArea) << "ms" << std::endl;
			*out << "\tTime Match Filter Dense Verify: " << std::to_string(timeMatchFilterDenseVerify) << "ms" << std::endl;
			*out << "\tTime Filter Frames: " << std::to_string(timeFilterFrames) << "ms" << std::endl;
			*out << "\tTime Add Curr Residuals: " << std::to_string(timeAddCurrResiduals) << "ms" << std::endl;
			*out << "\tTime Solve: " << std::to_string(timeSolve) << "ms" << std::endl;
			*out << "\t#iters solve: " << std::to_string(numItersSolve) << std::endl;
			if (printDepthSensing) {
				*out << "\tTime Re-Integrate: " << std::to_string(timeReIntegrate) << "ms" << std::endl;
				*out << "\tTime Reconstruct: " << std::to_string(timeReconstruct) << std::endl;
				*out << "\tTime Visualize: " << std::to_string(timeVisualize) << std::endl;
			}
		}
	};


	static void init()
	{
		resetTimings();
	}

	static void destroy()
	{
	}

	static void printAllTimings(const std::string& dir = "./")
	{
		const std::string read = dir + "timingLog.txt";
		printTimings(read);
		printExcelTimings(dir + "excel");
	}

	static void printTimings(const std::string& filename) 
	{
		//if(GlobalAppState::get().s_timingsDetailledEnabled)
		{
			std::ofstream outFile;
			bool logToFile = false;
			if (!filename.empty()) {
				logToFile = true;
				outFile.open(filename, std::ios::out);
			}
			std::ostream &out = (logToFile ? outFile : std::cout);
			
			out << "Global Timings Per Frame:" << std::endl;
			for (unsigned int i = 0; i < m_globalFrameTimings.size(); i++) {
				out << "[ frame " << i << " ]" << std::endl;
				m_globalFrameTimings[i].print(&out, false);
			}
			out << std::endl << std::endl;

			out << "Local Timings Per Frame:" << std::endl;
			for (unsigned int i = 0; i < m_localFrameTimings.size(); i++) {
				out << "[ frame " << i << " ]" << std::endl;
				m_localFrameTimings[i].print(&out, true);
			}

			if (m_totalFrameTimings.size() > 0) out << "Total Timings Per Frame:" << std::endl;
			for (unsigned int i = 0; i < m_totalFrameTimings.size(); i++) {
				out << "[ frame " << i << " ] " << m_totalFrameTimings[i] << " ms" << std::endl;
			}

			out << std::endl;
			out << std::endl;
		}
	}

	static void printExcelTimings(std::ofstream* out, const std::string& separator, std::vector<FrameTiming>& frameTimings, bool printDepthSensing)
	{
		*out << "SIFT Detection";
		for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeSiftDetection;
		*out << std::endl;
		*out << "SIFT Matching";
		for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeSiftMatching;
		*out << std::endl;
		*out << "Match Filter Key Point";
		for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeMatchFilterKeyPoint;
		*out << std::endl;
		*out << "Match Filter Surface Area";
		for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeMatchFilterSurfaceArea;
		*out << std::endl;
		*out << "Match Filter Dense Verify";
		for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeMatchFilterDenseVerify;
		*out << std::endl;
		*out << "Filter Frames";
		for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeFilterFrames;
		*out << std::endl;
		*out << "Add Curr Residuals";
		for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeAddCurrResiduals;
		*out << std::endl;
		*out << "Solve";
		for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeSolve;
		*out << std::endl;
		*out << "Solve #Iters";
		for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].numItersSolve;
		*out << std::endl;

		if (printDepthSensing) {
			*out << "Re-Integrate";
			for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeReIntegrate;
			*out << std::endl;
			*out << "Reconstruct";
			for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeReconstruct;
			*out << std::endl;
			*out << "Visualize";
			for (unsigned int i = 0; i < frameTimings.size(); i++) *out << separator << frameTimings[i].timeVisualize;
			*out << std::endl;
		}
	}

	static void printExcelTimings(const std::string& prefix)
	{
		const std::string separator = ",";

		const std::string globalFile = prefix + "_global.txt";
		{
			std::ofstream out(globalFile);
			printExcelTimings(&out, separator, m_globalFrameTimings, false);
		}
		const std::string localFile = prefix + "_local.txt";
		{
			std::ofstream out(localFile);
			printExcelTimings(&out, separator, m_localFrameTimings, true);
		}
	}

	static void printCurrentLocalFrame() 
	{
		if (m_localFrameTimings.empty()) return;

		std::ostream &out = std::cout;
		out << "[ frame " << m_localFrameTimings.size() - 1 << " ]" << std::endl;
		m_localFrameTimings.back().print(&out, true);
	}

	static void printCurrentGlobalFrame()
	{
		if (m_globalFrameTimings.empty()) return;

		std::ostream &out = std::cout;
		out << "[ frame " << m_globalFrameTimings.size() - 1 << " ]" << std::endl;
		m_globalFrameTimings.back().print(&out, false);
	}

	static void resetTimings()
	{
		m_localFrameTimings.clear();
		m_globalFrameTimings.clear();
		m_totalFrameTimings.clear();
	}

	static void addLocalFrameTiming()
	{
		m_localFrameTimings.push_back(FrameTiming());
	}
	static void addGlobalFrameTiming()
	{
		m_globalFrameTimings.push_back(FrameTiming());
	}

	static FrameTiming& getFrameTiming(bool local)
	{ 
		if (local) return m_localFrameTimings.back();
		else return m_globalFrameTimings.back();
	}

	static void addTotalFrameTime(double t)
	{
		m_totalFrameTimings.push_back(t);
	}

private:
	static std::vector<FrameTiming> m_localFrameTimings;
	static std::vector<FrameTiming> m_globalFrameTimings;
	static std::vector<double> m_totalFrameTimings;
};
