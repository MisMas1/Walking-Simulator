#include "Tools.hpp"

#include <iomanip>
#include <istream>
#include <ostream>

// ---------- Flashlight ----------

Flashlight::Flashlight() : Tool(ToolType::Flashlight, "Flashlight") {}

Flashlight::Flashlight(float _radius, float _intensity)
    : Tool(ToolType::Flashlight, "Flashlight"), radius(_radius), intensity(_intensity) {}

Flashlight::Flashlight(const Flashlight &other)
    : Tool(other), radius(other.radius), intensity(other.intensity),
      flickerChance(other.flickerChance), isFlickering(other.isFlickering) {}

Flashlight &Flashlight::operator=(const Flashlight &other)
{
    if (this != &other)
    {
        Tool::operator=(other);
        radius = other.radius;
        intensity = other.intensity;
        flickerChance = other.flickerChance;
        isFlickering = other.isFlickering;
    }
    return *this;
}

void Flashlight::Use() {}
void Flashlight::Update(float deltaTime) {}

std::unique_ptr<Tool> Flashlight::Clone() const { return std::make_unique<Flashlight>(*this); }

bool Flashlight::Equals(const Tool &other) const
{
    const Flashlight *o = dynamic_cast<const Flashlight *>(&other);
    return o != nullptr && Tool::Equals(other) && radius == o->radius &&
           intensity == o->intensity && flickerChance == o->flickerChance &&
           isFlickering == o->isFlickering;
}

void Flashlight::Print(std::ostream &os) const
{
    Tool::Print(os);
    os << ' ' << radius << ' ' << intensity << ' ' << flickerChance << ' ' << isFlickering;
}

void Flashlight::Read(std::istream &is)
{
    Tool::Read(is);
    is >> radius >> intensity >> flickerChance >> isFlickering;
}

// ---------- Photo ----------

Photo::Photo() {}

Photo::Photo(const Vec3 &_position, bool _anomalyCaptured, const std::string &_description)
    : position(_position), anomalyCaptured(_anomalyCaptured), description(_description) {}

Photo::Photo(const Photo &other)
    : position(other.position), anomalyCaptured(other.anomalyCaptured), description(other.description) {}

Photo &Photo::operator=(const Photo &other)
{
    if (this != &other)
    {
        position = other.position;
        anomalyCaptured = other.anomalyCaptured;
        description = other.description;
    }
    return *this;
}

bool Photo::operator==(const Photo &other) const
{
    return position == other.position && anomalyCaptured == other.anomalyCaptured &&
           description == other.description;
}

bool Photo::operator!=(const Photo &other) const { return !(*this == other); }

std::ostream &operator<<(std::ostream &os, const Photo &photo)
{
    return os << photo.position << ' ' << photo.anomalyCaptured << ' ' << std::quoted(photo.description);
}

std::istream &operator>>(std::istream &is, Photo &photo)
{
    return is >> photo.position >> photo.anomalyCaptured >> std::quoted(photo.description);
}

// ---------- PolaroidCamera ----------

PolaroidCamera::PolaroidCamera() : Tool(ToolType::PolaroidCamera, "Polaroid Camera") {}

PolaroidCamera::PolaroidCamera(int _photosLeft, float _flashCooldown)
    : Tool(ToolType::PolaroidCamera, "Polaroid Camera"), photosLeft(_photosLeft), flashCooldown(_flashCooldown) {}

PolaroidCamera::PolaroidCamera(const PolaroidCamera &other)
    : Tool(other), photosLeft(other.photosLeft), photos(other.photos), flashCooldown(other.flashCooldown) {}

PolaroidCamera &PolaroidCamera::operator=(const PolaroidCamera &other)
{
    if (this != &other)
    {
        Tool::operator=(other);
        photosLeft = other.photosLeft;
        photos = other.photos;
        flashCooldown = other.flashCooldown;
    }
    return *this;
}

void PolaroidCamera::Use() {}
void PolaroidCamera::Update(float deltaTime) {}

std::unique_ptr<Tool> PolaroidCamera::Clone() const { return std::make_unique<PolaroidCamera>(*this); }

bool PolaroidCamera::Equals(const Tool &other) const
{
    const PolaroidCamera *o = dynamic_cast<const PolaroidCamera *>(&other);
    return o != nullptr && Tool::Equals(other) && photosLeft == o->photosLeft &&
           photos == o->photos && flashCooldown == o->flashCooldown;
}

void PolaroidCamera::Print(std::ostream &os) const
{
    Tool::Print(os);
    os << ' ' << photosLeft << ' ' << flashCooldown << ' ' << photos.size();
    for (const Photo &photo : photos)
        os << ' ' << photo;
}

void PolaroidCamera::Read(std::istream &is)
{
    Tool::Read(is);

    std::size_t count = 0;
    is >> photosLeft >> flashCooldown >> count;

    photos.clear();
    for (std::size_t i = 0; i < count; ++i)
    {
        Photo photo;
        if (!(is >> photo))
            return;
        photos.push_back(photo);
    }
}

// ---------- EMFMeter ----------

EMFMeter::EMFMeter() : Tool(ToolType::EMFMeter, "EMF Meter") {}

EMFMeter::EMFMeter(float _detectionRange, float _sensitivity)
    : Tool(ToolType::EMFMeter, "EMF Meter"), detectionRange(_detectionRange), sensitivity(_sensitivity) {}

EMFMeter::EMFMeter(const EMFMeter &other)
    : Tool(other), currentLevel(other.currentLevel), detectionRange(other.detectionRange),
      sensitivity(other.sensitivity) {}

EMFMeter &EMFMeter::operator=(const EMFMeter &other)
{
    if (this != &other)
    {
        Tool::operator=(other);
        currentLevel = other.currentLevel;
        detectionRange = other.detectionRange;
        sensitivity = other.sensitivity;
    }
    return *this;
}

void EMFMeter::Use() {}
void EMFMeter::Update(float deltaTime) {}

std::unique_ptr<Tool> EMFMeter::Clone() const { return std::make_unique<EMFMeter>(*this); }

bool EMFMeter::Equals(const Tool &other) const
{
    const EMFMeter *o = dynamic_cast<const EMFMeter *>(&other);
    return o != nullptr && Tool::Equals(other) && currentLevel == o->currentLevel &&
           detectionRange == o->detectionRange && sensitivity == o->sensitivity;
}

void EMFMeter::Print(std::ostream &os) const
{
    Tool::Print(os);
    os << ' ' << currentLevel << ' ' << detectionRange << ' ' << sensitivity;
}

void EMFMeter::Read(std::istream &is)
{
    Tool::Read(is);
    is >> currentLevel >> detectionRange >> sensitivity;
}

// ---------- SpiritPod ----------

SpiritPod::SpiritPod() : Tool(ToolType::SpiritPod, "Spirit Pod") {}

SpiritPod::SpiritPod(float _responseChance, float _range)
    : Tool(ToolType::SpiritPod, "Spirit Pod"), responseChance(_responseChance), range(_range) {}

SpiritPod::SpiritPod(const SpiritPod &other)
    : Tool(other), isListening(other.isListening), responseChance(other.responseChance),
      lastResponse(other.lastResponse), range(other.range) {}

SpiritPod &SpiritPod::operator=(const SpiritPod &other)
{
    if (this != &other)
    {
        Tool::operator=(other);
        isListening = other.isListening;
        responseChance = other.responseChance;
        lastResponse = other.lastResponse;
        range = other.range;
    }
    return *this;
}

void SpiritPod::Use() {}
void SpiritPod::Update(float deltaTime) {}

std::unique_ptr<Tool> SpiritPod::Clone() const { return std::make_unique<SpiritPod>(*this); }

bool SpiritPod::Equals(const Tool &other) const
{
    const SpiritPod *o = dynamic_cast<const SpiritPod *>(&other);
    return o != nullptr && Tool::Equals(other) && isListening == o->isListening &&
           responseChance == o->responseChance && lastResponse == o->lastResponse &&
           range == o->range;
}

void SpiritPod::Print(std::ostream &os) const
{
    Tool::Print(os);
    os << ' ' << isListening << ' ' << responseChance << ' ' << std::quoted(lastResponse) << ' ' << range;
}

void SpiritPod::Read(std::istream &is)
{
    Tool::Read(is);
    is >> isListening >> responseChance >> std::quoted(lastResponse) >> range;
}

// ---------- DowsingRods ----------

DowsingRods::DowsingRods() : Tool(ToolType::DowsingRods, "Dowsing Rods") {}

DowsingRods::DowsingRods(float _detectionRange)
    : Tool(ToolType::DowsingRods, "Dowsing Rods"), detectionRange(_detectionRange) {}

DowsingRods::DowsingRods(const DowsingRods &other)
    : Tool(other), angle(other.angle), pointingAt(other.pointingAt), detectionRange(other.detectionRange) {}

DowsingRods &DowsingRods::operator=(const DowsingRods &other)
{
    if (this != &other)
    {
        Tool::operator=(other);
        angle = other.angle;
        pointingAt = other.pointingAt;
        detectionRange = other.detectionRange;
    }
    return *this;
}

void DowsingRods::Use() {}
void DowsingRods::Update(float deltaTime) {}

std::unique_ptr<Tool> DowsingRods::Clone() const { return std::make_unique<DowsingRods>(*this); }

bool DowsingRods::Equals(const Tool &other) const
{
    const DowsingRods *o = dynamic_cast<const DowsingRods *>(&other);
    return o != nullptr && Tool::Equals(other) && angle == o->angle &&
           pointingAt == o->pointingAt && detectionRange == o->detectionRange;
}

void DowsingRods::Print(std::ostream &os) const
{
    Tool::Print(os);
    os << ' ' << angle << ' ' << pointingAt << ' ' << detectionRange;
}

void DowsingRods::Read(std::istream &is)
{
    Tool::Read(is);
    is >> angle >> pointingAt >> detectionRange;
}

// ---------- Functii ajutatoare ----------

std::unique_ptr<Tool> MakeTool(ToolType type)
{
    switch (type)
    {
    case ToolType::Flashlight:
        return std::make_unique<Flashlight>();
    case ToolType::PolaroidCamera:
        return std::make_unique<PolaroidCamera>();
    case ToolType::EMFMeter:
        return std::make_unique<EMFMeter>();
    case ToolType::SpiritPod:
        return std::make_unique<SpiritPod>();
    case ToolType::DowsingRods:
        return std::make_unique<DowsingRods>();
    }
    return nullptr;
}

std::unique_ptr<Tool> ReadTool(std::istream &is)
{
    ToolType type;
    if (!(is >> type))
        return nullptr;

    std::unique_ptr<Tool> tool = MakeTool(type);
    if (!tool)
    {
        is.setstate(std::ios::failbit);
        return nullptr;
    }

    tool->Read(is);
    if (!is)
        return nullptr;

    return tool;
}