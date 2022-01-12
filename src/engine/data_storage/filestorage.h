#pragma once
#include "saveable.h"

#if defined(_MSC_VER)
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "saveable.h"

class FileStorage {
public:
	static std::vector<FileStorage*> storages;

	FileStorage(const std::string& filename);

	/**
	 * @brief Save all Saveable objects to a file.
	 * 
	 * @param filepath The file path to prepend to the filename.
	 * @return An error otherwise 0.
	 */
	int save(const std::string& filepath) const;

	/**
	 * @brief Load all Saveable objects from a file.
	 * 
	 * @param filepath The file path to prepend to the filename.
	 * @return An error otherwise 0.
	 */
	int load(const std::string& filepath);

	/**
	 * @brief Add a Saveable and load the data if it matches an unloaded batch of data.
	 * 
	 * @param saveable The Saveable reference to be added.
	 */
	void addSaveable(std::shared_ptr<Saveable> saveable);
	
	/**
	 * @brief Remove a Saveable matching the identifier.
	 * 
	 * @param identifier The identifier to find and remove.
	 */
	void removeSaveable(const std::string& identifier);
	
	/**
	 * @brief Remove a Saveable matching the Saveable's identifier.
	 * 
	 * @param saveable The Saveable to find.
	 */
	void removeSaveable(std::shared_ptr<Saveable> saveable);

private:
	struct Unloaded {
		const std::string identifier;
		const void* data;
		size_t size;
		Unloaded(const std::string& identifier, const void* data, size_t size);
		~Unloaded();
	};
	bool loaded = false;
	std::string filename;
	std::vector<std::shared_ptr<Saveable>> saveables;
	std::list<Unloaded> unloaded;
	
};

namespace DataStorage {
	void SaveAll(const std::string& filepath);
	void LoadAll(const std::string& filepath);
}
