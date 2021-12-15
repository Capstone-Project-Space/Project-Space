#pragma once

#include <cstdint>
#include <string>

class Saveable {
public:
	Saveable(const std::string& identifier);

	/**
	 * @brief This is meant to save this Saveable object to the passed buffer.
	 * 
	 * @param buffer The buffer to write the data into.
	 */
	virtual void save(void* buffer) const;

	/**
	 * @brief This is meant to load this Saveableobject from the passed buffer.
	 * 
	 * @param buffer The buffer containing the saved data.
	 * @param size The size of the buffer.
	 */
	virtual void load(const void* buffer, size_t size);
	
	/**
	 * @brief Get the size of the data which would be saved.
	 * 
	 * @return The size of the data which would be saved.
	 */
	virtual size_t getSize();

	/**
	 * @brief Get the identifier of this Saveable.
	 * 
	 * @return The identifier of this Saveable.
	 */
	const std::string& getIdentifier() const;

protected:
	std::string identifier;
};
