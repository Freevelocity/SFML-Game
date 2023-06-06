#pragma once

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename Resource, typename Identifier>
class SpriteHolder
{
	public:
		void SLoad(Identifier pId, const std::string& pFilename);

		template <typename Parameter>
		void SLoad(Identifier pId, const std::string& pFilename, const Parameter& pSecondParam);

		Resource& get(Identifier pId);
		const Resource& get(Identifier pId) const;

	private:
		void InsertResource(Identifier pId, std::unique_ptr<Resource> pResource);

	private:
		std::map<Identifier, std::unique_ptr<Resource>>	m_oResourceMap;

};

#include "SpriteHolder.inl"

