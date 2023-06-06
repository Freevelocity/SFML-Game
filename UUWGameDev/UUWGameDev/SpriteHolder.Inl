#pragma once


template <typename Resource, typename Identifier>
void SpriteHolder<Resource, Identifier>::SLoad(Identifier pId, const std::string& pFilename)
{

	std::unique_ptr<Resource> oRes(new Resource());
	if (!oRes->loadFromFile(pFilename))
		throw std::runtime_error("SpriteHolder::SLoad - Failed to load " + pFilename);
	InsertResource(pId, std::move(oRes));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void SpriteHolder<Resource, Identifier>::SLoad(Identifier pId, const std::string& pFilename, const Parameter& pSecondParam)
{

	std::unique_ptr<Resource> oRes(new Resource());
	if (!oRes->loadFromFile(pFilename, pSecondParam))
		throw std::runtime_error("SpriteHolder::Sload - Failed to load " + pFilename);
	InsertResource(pId, std::move(oRes));
}

template <typename Resource, typename Identifier>
Resource& SpriteHolder<Resource, Identifier>::get(Identifier pId)
{
	auto oResExists = m_oResourceMap.find(pId);
	assert(oResExists != m_oResourceMap.end());

	return *oResExists->second;
}

template <typename Resource, typename Identifier>
const Resource& SpriteHolder<Resource, Identifier>::get(Identifier pId) const
{
	auto oResExists = m_oResourceMap.find(pId);
	assert(oResExists != m_oResourceMap.end());

	return *oResExists->second;
}

template <typename Resource, typename Identifier>
void SpriteHolder<Resource, Identifier>::InsertResource(Identifier pId, std::unique_ptr<Resource> pResource)
{
	auto inserted = m_oResourceMap.insert(std::make_pair(pId, std::move(pResource)));
	assert(inserted.second);
}

