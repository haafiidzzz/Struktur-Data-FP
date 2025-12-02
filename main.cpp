#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

struct Partikel {
    sf::Vector2f pos;
    sf::Vector2f vel;
    float radius;
    sf::CircleShape bentuk;

    Partikel(float x, float y, float r) : pos(x, y), radius(r) {
        vel = sf::Vector2f(
            (rand() % 100 - 50) / 20,
            (rand() % 100 - 50) / 20
        );

        bentuk.setRadius(radius);
        bentuk.setOrigin(radius, radius);
        bentuk.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        bentuk.setPosition(pos);
    }

    void update() {
        pos += vel;
        bentuk.setPosition(pos);
    }

    void mantul(int w, int h) {
        if (pos.x - radius < 0 || pos.x + radius > w) vel.x *= -1;
        if (pos.y - radius < 0 || pos.y + radius > h) vel.y *= -1;
    }
};

bool cekTabrakan(Partikel& a, Partikel& b) {
    float dx = a.pos.x - b.pos.x;
    float dy = a.pos.y - b.pos.y;
    float jarak = std::sqrt(dx*dx + dy*dy);
    return jarak < (a.radius + b.radius);
}

void tanganiTabrakan(Partikel& a, Partikel& b) {
    sf::Vector2f delta = b.pos - a.pos;
    float jarak = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    float minJarak = a.radius + b.radius;//jarak minimum sebelum tabrakan

    if (jarak == 0) return;

    float overlap = 0.5f * (minJarak - jarak);//besar tumpang tindih
    delta /= jarak;

    a.pos -= delta * overlap;//menggeser posisi partikel a untuk menghindari tumpang tindih
    b.pos += delta * overlap;//menggeser posisi partikel b untuk menghindari tumpang tindih

    sf::Vector2f relVel = b.vel - a.vel;
    float velNormal = relVel.x * delta.x + relVel.y * delta.y;//kecepatan relatif sepanjang normal
    if (velNormal > 0) return;

    float j = -(1 + 1.0f) * velNormal / 2.0f;
    sf::Vector2f impuls = delta * j;//impuls yang diterapkan pada partikel  

    a.vel -= impuls;
    b.vel += impuls;
}

void bruteForceCollision(std::vector<Partikel>& list) {
    for (int i = 0; i < list.size(); i++) {
        for (int j = i + 1; j < list.size(); j++) {
            if (cekTabrakan(list[i], list[j])) {
                tanganiTabrakan(list[i], list[j]);
            }
        }
    }
}

class Quadtree {
public:
    int level;
    sf::FloatRect bounds;
    std::vector<Partikel*> objects;
    Quadtree* nodes[4];
    const int MAX_OBJECTS = 5;
    const int MAX_LEVELS = 5;

    Quadtree(int level, sf::FloatRect bounds) : level(level), bounds(bounds) {
        for (int i = 0; i < 4; i++) nodes[i] = nullptr;
    }

    ~Quadtree() {
        for (int i = 0; i < 4; i++)
            if (nodes[i] != nullptr) delete nodes[i];
    }

    void clear() {//menghapus semua objek dan node anak
        objects.clear();
        for (int i = 0; i < 4; i++) {
            if (nodes[i] != nullptr) {
                nodes[i]->clear();
                delete nodes[i];
                nodes[i] = nullptr;
            }
        }
    }

    void split() {//membagi node menjadi 4 node anak
        float x = bounds.left;
        float y = bounds.top;
        float w = bounds.width / 2;
        float h = bounds.height / 2;

        nodes[0] = new Quadtree(level+1, {x,     y,     w, h});
        nodes[1] = new Quadtree(level+1, {x+w,   y,     w, h});
        nodes[2] = new Quadtree(level+1, {x,     y+h,   w, h});
        nodes[3] = new Quadtree(level+1, {x+w,   y+h,   w, h});
    }

    int getIndex(Partikel* p) {
        int index = -1;
        float midX = bounds.left + bounds.width / 2;
        float midY = bounds.top + bounds.height / 2;

        bool top = p->pos.y - p->radius < midY && p->pos.y + p->radius < midY;
        bool bottom = p->pos.y - p->radius > midY;

        if (p->pos.x + p->radius < midX) {
            if (top) index = 0;
            else if (bottom) index = 2;
        } else if (p->pos.x - p->radius > midX) {
            if (top) index = 1;
            else if (bottom) index = 3;
        }
        return index;
    }

    void insert(Partikel* p) {//memasukkan partikel ke dalam quadtree
        if (nodes[0] != nullptr) {
            int index = getIndex(p);
            if (index != -1) {
                nodes[index]->insert(p);
                return;
            }
        }

        objects.push_back(p);

        if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
            if (nodes[0] == nullptr) split();

            int i = 0;
            while (i < objects.size()) {
                int index = getIndex(objects[i]);
                if (index != -1) {
                    nodes[index]->insert(objects[i]);
                    objects.erase(objects.begin() + i);
                } else i++;
            }
        }
    }

    void retrieve(std::vector<Partikel*>& returnObjects, Partikel* p) {
        int index = getIndex(p);//mendapatkan indeks node anak tempat partikel 
        if (index != -1 && nodes[0] != nullptr)
            nodes[index]->retrieve(returnObjects, p);

        for (auto& obj : objects)
            returnObjects.push_back(obj);
    }
};

void quadtreeCollision(std::vector<Partikel>& list) {
    Quadtree qt(0, sf::FloatRect(0, 0, 800, 600));

    for (auto& p : list)
        qt.insert(&p);

    std::vector<Partikel*> candidates;

    for (int i = 0; i < list.size(); i++) {
        candidates.clear();
        qt.retrieve(candidates, &list[i]);

        for (auto* other : candidates) {
            if (&list[i] != other && cekTabrakan(list[i], *other)) {
                tanganiTabrakan(list[i], *other);
            }
        }
    }
}

int main() {
    srand(time(NULL));

    const int W = 800, H = 600;
    sf::RenderWindow window(sf::VideoMode(W, H), "Collision Simulation");

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(18);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(10, 10);

    std::vector<Partikel> partikelList;
    for (int i = 0; i < 4000; i++) {
        partikelList.emplace_back(rand() % W, rand() % H,2.0f + rand() % 8);
    }

    bool useQuadtree = false;  

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q) {
                    useQuadtree = !useQuadtree;
                    std::cout << (useQuadtree ? "Mode: Quadtree\n" : "Mode: BruteForce\n");
                }
            }
        }

        for (auto& p : partikelList) {//perbarui posisi dan cek pantulan
            p.update();
            p.mantul(W, H);
        }

        if (useQuadtree)
            quadtreeCollision(partikelList);
        else
            bruteForceCollision(partikelList);

        statusText.setString(
            useQuadtree ? "Mode: Quadtree (press Q to switch)" 
                        : "Mode: BruteForce (press Q to switch)"
        );

        window.clear();//draw partikel dan status
        for (auto& p : partikelList)
            window.draw(p.bentuk);
        window.draw(statusText);
        window.display();
    }

    return 0;
}
