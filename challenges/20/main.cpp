#include <algorithm>
#include <cassert>
#include <cmath>
#include <compare>
#include <iostream>
#include <optional>
#include <ostream>
#include <regex>
#include <span>
#include <set>

#include "algo.h"
#include "input.h"

int sign(int64_t n1) {
    return n1 >= 0 ? 1 : -1;
}
bool sameSign(int64_t n1, int64_t n2, int64_t n3){
    return (sign(n1) == sign(n2) || n2==0 || n1==0) && (sign(n2) == sign(n3) || n2 == 0 || n3 == 0);
}

struct XYZ{
    int64_t x,y,z;

    uint64_t getManhattanDistance() const{
        return abs(x) + abs(y) + abs(z);
    }

    friend std::ostream& operator<<(std::ostream& stream, XYZ xyz){
        stream << xyz.x << "," << xyz.y << "," << xyz.z;
        return stream;
    };

    bool operator==(const XYZ& xyz2) const = default;
    
};

std::optional<int> getLargestQuadraticSolution(int64_t acc1, int64_t acc2, int64_t startingVel1, int64_t startingVel2, int64_t startingPos1, int64_t startingPos2, uint64_t time){
    auto a = 0.5*acc1 - 0.5*acc2;
    auto b = (startingVel1 + .5*acc1) - (startingVel2 + .5*acc2);
    auto c = startingPos1 - startingPos2;

    auto determinant = b*b - 4*a*c;
    if(determinant < 0){
        return {};
    }

    auto answer1 = (-1*b + sqrt(determinant)) / (2*a);
    if (determinant == 0){
        return {answer1};
    }

    auto answer2 = (-1*b - sqrt(determinant)) / (2*a);
    return {std::max(answer1, answer2)};


}

class Particle {
public:
    Particle(unsigned int id, int posx, int posy, int posz, int velx, int vely, int velz, int accx, int accy, int accz) : id(id), pos{posx, posy, posz}, startingPos{pos}, vel{velx, vely, velz}, startingVel{vel}, acc{accx, accy, accz}  {}

    unsigned int getManhattanDistance() const {
        return pos.getManhattanDistance();
    }

    bool isApproachingOrigin() const {
        return !sameSign(pos.x, vel.x, acc.x) || !sameSign(pos.y, vel.y, acc.y) || !sameSign(pos.z, vel.z, acc.z);
    }

    void advance(unsigned int timeToAdvance=1) {
        this->time += timeToAdvance;
        this->vel = {
            static_cast<int64_t>(this->acc.x*this->time + this->startingVel.x),
            static_cast<int64_t>(this->acc.y*this->time + this->startingVel.y),
            static_cast<int64_t>(this->acc.z*this->time + this->startingVel.z),
        };
        this->pos = getPositionAtTime(this->time);
    }

    uint64_t getIntersectionTimeFloor(const Particle& particle2) const{
        // ax^2 + bx + c - solve both equations
        assert(this->time == particle2.time);
        auto xTime = getLargestQuadraticSolution(this->acc.x, particle2.acc.x, this->startingVel.x, particle2.startingVel.x, this->startingPos.x, particle2.startingPos.x, this->time);
        auto yTime = getLargestQuadraticSolution(this->acc.y, particle2.acc.y, this->startingVel.y, particle2.startingVel.y, this->startingPos.y, particle2.startingPos.y, this->time);
        auto zTime = getLargestQuadraticSolution(this->acc.z, particle2.acc.z, this->startingVel.z, particle2.startingVel.z, this->startingPos.z, particle2.startingPos.z, this->time);

        return std::max({floor(xTime.value_or(this->time)), floor(yTime.value_or(this->time)), floor(zTime.value_or(this->time))});

    }

    XYZ getPositionAtTime(uint64_t time) const{
        // 1/2 at^2 + (v0 + a) t + -V0 P0
        return {
            static_cast<int64_t>((0.5 * this->acc.x * time*time) + (this->startingVel.x + .5 * this->acc.x)*time + this->startingPos.x),
            static_cast<int64_t>((0.5 * this->acc.y * time*time) + (this->startingVel.y + .5 * this->acc.y)*time + this->startingPos.y),
            static_cast<int64_t>((0.5 * this->acc.z * time*time) + (this->startingVel.z + .5 * this->acc.z)*time + this->startingPos.z)
        };
    }

    friend std::ostream& operator<<(std::ostream& stream, const Particle& p) {
        stream << "Particle #"<<p.id<<" pos=("<<p.pos<<") vel=("<<p.vel<<") starting acc=("<<p.acc<<")@time{"<<p.time<<"}";
        return stream;
    }

    uint64_t getTime() const { 
        return this->time;
    }

    int getID() const { 
        return this->id;
    }

private:
    unsigned int id;
    XYZ pos;
    XYZ startingPos;
    XYZ vel;
    XYZ startingVel;
    XYZ acc;
    uint64_t time = 0;
};

Particle toParticle(const std::string& text) {
    static unsigned int counter = 0;
    std::regex particleRegex { "p=<(-?\\d+),(-?\\d+),(-?\\d+)>, v=<(-?\\d+),(-?\\d+),(-?\\d+)>, a=<(-?\\d+),(-?\\d+),(-?\\d+)>" };
    std::smatch match;

    if(std::regex_search(text, match, particleRegex)){
        return Particle {
            counter++,
            std::stoi(match[1]),
            std::stoi(match[2]),
            std::stoi(match[3]),
            std::stoi(match[4]),
            std::stoi(match[5]),
            std::stoi(match[6]),
            std::stoi(match[7]),
            std::stoi(match[8]),
            std::stoi(match[9])
        };
    }

    assert(false);
    return Particle{0,0,0,0,0,0,0,0,0,0};
}

std::pair<bool, uint64_t> willBeCloserToOrigin(const Particle& particle1, const Particle& particle2) {
    auto intersectionTime = particle1.getIntersectionTimeFloor(particle2);
    assert(particle1.getTime() == particle2.getTime());
    auto latestTime = std::max(intersectionTime, particle1.getTime());
    auto xyz1 = particle1.getPositionAtTime(latestTime+1);
    auto xyz2 = particle2.getPositionAtTime(latestTime+1);
    return {xyz1.getManhattanDistance() < xyz2.getManhattanDistance(), latestTime};
    
}

std::pair<Particle, uint64_t> getClosestToOrigin(std::span<Particle> particles) {
    assert(particles.size() > 0);
    uint64_t time=0;
    auto element = std::ranges::min_element(particles, [&time](const Particle& particle1, const Particle& particle2) { 
        auto [result, candidateTime] = willBeCloserToOrigin(particle1, particle2);
        time = std::max(time, candidateTime);
        return result;
    });
    return {*element, time};
}

void runUntilMovingAwayFromOrigin(std::span<Particle> particles) {
    while(std::ranges::any_of(particles, [](const Particle& p){ return p.isApproachingOrigin(); })) {
        std::ranges::for_each(particles, [](Particle& p) { p.advance(1); });
    }
}

size_t getNumberOfNonCollidingParticles(std::span<Particle> particles, uint64_t timeToAdvance) {
    std::set<unsigned int> collided{};
    for(uint64_t time=0; time<=timeToAdvance; ++time){
        std::ranges::for_each(particles, [&collided,&particles,time](const Particle& particle){
            if (collided.find(particle.getID()) != collided.end()) { return; }
            // check every single other particle
            std::ranges::for_each(particles, [&collided,time,particle](const Particle& candidate) {
                if(particle.getID() == candidate.getID() || collided.find(candidate.getID()) != collided.end()) { return; }
                if(particle.getPositionAtTime(time) == candidate.getPositionAtTime(time)) {
                    collided.insert(candidate.getID());
                    collided.insert(particle.getID());
                }
                
            });
        });
    }
    return particles.size() - collided.size();

}

int main() { 
    auto particles = algo::map(input::readMultiLineFile("input/input20.txt"), toParticle);
    runUntilMovingAwayFromOrigin(particles);
    auto [particle, time] = getClosestToOrigin(particles);
    std::cout << particle << "\n";

    std::cout << "Non-colliding particles: " << getNumberOfNonCollidingParticles(particles, time) << "\n";

    return 0;
}