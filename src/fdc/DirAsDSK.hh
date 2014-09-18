#ifndef DIRASDSK_HH
#define DIRASDSK_HH

#include "SectorBasedDisk.hh"
#include "DiskImageUtils.hh"
#include "FileOperations.hh"
#include "EmuTime.hh"
#include <map>

namespace openmsx {

class DiskChanger;
class CliComm;

class DirAsDSK final : public SectorBasedDisk
{
public:
	enum SyncMode { SYNC_READONLY, SYNC_FULL };
	enum BootSectorType { BOOTSECTOR_DOS1, BOOTSECTOR_DOS2 };

public:
	DirAsDSK(DiskChanger& diskChanger, CliComm& cliComm,
	         const Filename& hostDir, SyncMode syncMode,
	         BootSectorType bootSectorType);

	// SectorBasedDisk
	virtual void readSectorImpl (size_t sector,       SectorBuffer& buf);
	virtual void writeSectorImpl(size_t sector, const SectorBuffer& buf);
	virtual bool isWriteProtectedImpl() const;
	virtual void checkCaches();

private:
	struct DirIndex {
		DirIndex() {}
		DirIndex(unsigned sector_, unsigned idx_)
			: sector(sector_), idx(idx_) {}
		bool operator<(const DirIndex& rhs) const {
			if (sector != rhs.sector) return sector < rhs.sector;
			return idx < rhs.idx;
		}
		unsigned sector;
		unsigned idx;
	};
	struct MapDir {
		std::string hostName; // path relative to 'hostDir'
		// The following two are used to detect changes in the host
		// file compared to the last host->virtual-disk sync.
		time_t mtime; // Modification time of host file at the time of
		              // the last sync.
		size_t filesize; // Host file size, normally the same as msx
		                 // filesize, except when the host file was
		                 // truncated.
	};

	SectorBuffer* fat();
	SectorBuffer* fat2();
	MSXDirEntry& msxDir(DirIndex dirIndex);
	void writeFATSector (unsigned sector, const SectorBuffer& buf);
	void writeDIRSector (unsigned sector, DirIndex dirDirIndex,
	                     const SectorBuffer& buf);
	void writeDataSector(unsigned sector, const SectorBuffer& buf);
	void writeDIREntry(DirIndex dirIndex, DirIndex dirDirIndex,
	                   const MSXDirEntry& newEntry);
	void syncWithHost();
	void checkDeletedHostFiles();
	void deleteMSXFile(DirIndex dirIndex);
	void deleteMSXFilesInDir(unsigned msxDirSector);
	void freeFATChain(unsigned cluster);
	void addNewHostFiles(const std::string& hostSubDir, unsigned msxDirSector);
	void addNewDirectory(const std::string& hostSubDir, const std::string& hostName,
                             unsigned msxDirSector, FileOperations::Stat& fst);
	void addNewHostFile(const std::string& hostSubDir, const std::string& hostName,
	                    unsigned msxDirSector, FileOperations::Stat& fst);
	DirIndex fillMSXDirEntry(
		const std::string& hostSubDir, const std::string& hostName,
		unsigned msxDirSector);
	DirIndex getFreeDirEntry(unsigned msxDirSector);
	DirIndex findHostFileInDSK(const std::string& hostName);
	bool checkFileUsedInDSK(const std::string& hostName);
	unsigned nextMsxDirSector(unsigned sector);
	bool checkMSXFileExists(const std::string& msxfilename,
	                        unsigned msxDirSector);
	void checkModifiedHostFiles();
	void setMSXTimeStamp(DirIndex dirIndex, FileOperations::Stat& fst);
	void importHostFile(DirIndex dirIndex, FileOperations::Stat& fst);
	void exportToHost(DirIndex dirIndex, DirIndex dirDirIndex);
	void exportToHostDir (DirIndex dirIndex, const std::string& hostName);
	void exportToHostFile(DirIndex dirIndex, const std::string& hostName);
	unsigned findNextFreeCluster(unsigned cluster);
	unsigned findFirstFreeCluster();
	unsigned getFreeCluster();
	unsigned readFAT(unsigned cluster);
	void writeFAT12(unsigned cluster, unsigned val);
	void exportFileFromFATChange(unsigned cluster, SectorBuffer* oldFAT);
	unsigned getChainStart(unsigned cluster, unsigned& chainLength);
	bool isDirSector(unsigned sector, DirIndex& dirDirIndex);
	bool getDirEntryForCluster(unsigned cluster,
	                           DirIndex& dirIndex, DirIndex& dirDirIndex);
	DirIndex getDirEntryForCluster(unsigned cluster);
	void unmapHostFiles(unsigned msxDirSector);
	template<typename FUNC> bool scanMsxDirs(
		FUNC func, unsigned msxDirSector);
	friend struct NullScanner;
	friend struct DirScanner;
	friend struct IsDirSector;
	friend struct DirEntryForCluster;
	friend struct UnmapHostFiles;

	// internal helper functions
	unsigned readFATHelper(const SectorBuffer* fat, unsigned cluster) const;
	void writeFATHelper(SectorBuffer* fat, unsigned cluster, unsigned val) const;
	unsigned clusterToSector(unsigned cluster) const;
	void sectorToCluster(unsigned sector, unsigned& cluster, unsigned& offset) const;
	unsigned sectorToCluster(unsigned sector) const;

private:
	DiskChanger& diskChanger; // used to query time / report disk change
	CliComm& cliComm; // TODO don't use CliComm to report errors/warnings
	const std::string hostDir;
	const SyncMode syncMode;

	EmuTime lastAccess; // last time there was a sector read/write

	// For each directory entry that has a mapped host file/directory we
	// store the name, last modification time and size of the corresponding
	// host file/dir.
	typedef std::map<DirIndex, MapDir> MapDirs;
	MapDirs mapDirs;

	// format parameters which depend on single/double sided
	// varying root parameters
	const unsigned nofSectors;
	const unsigned nofSectorsPerFat;
	// parameters that depend on these and thus also vary
	const unsigned firstSector2ndFAT;
	const unsigned firstDirSector;
	const unsigned firstDataSector;
	const unsigned maxCluster; // First cluster number that can NOT be used anymore.

	// Storage for the whole virtual disk.
	std::vector<SectorBuffer> sectors;

};

} // namespace openmsx

#endif
