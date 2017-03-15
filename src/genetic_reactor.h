/**
 *  @autor tekatod
 *  @date 3/13/17
 */
#pragma once

#include <vector>
#include <cstddef>
#include <random>
#include <algorithm>
#include <fstream>


class genetic_reactor {
public:
    genetic_reactor(size_t generations_count, size_t dna_size, size_t generation_size, double mutation_rate) :
            m_generations_count(generations_count), m_dna_size(dna_size), m_generation_size(generation_size),
            m_mutants_count(mutation_rate * generation_size), m_current_dna(0), m_generation_number(1) {
        m_generation.reserve(generation_size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distribution(0.0, m_dna_size * 2);
        for (size_t i = 0; i < generation_size; ++i) {
            creature c;
            c.dna.reserve(m_dna_size);
            for (size_t j = 0; j < m_dna_size; ++j) {
                c.dna.push_back(distribution(gen));
            }
            c.score = 0.0;
            m_generation.push_back(c);
        }
        this->get_next_dna(false);
        m_best_log.open("/home/tekatod/develop/cppstrategy2016/best_log.txt", std::ios::trunc);
    }


    void switch_to_new_generation() {
        this->write_best();
        this->reproduce();
        if(m_mutants_count > 0) {
            this->mutate();
        }
        ++m_generation_number;
        m_current_dna = 0;
        this->get_next_dna(false);
    }

    size_t get_generation_size() const {
        return m_generation_size;
    }

    size_t get_generations_count() const {
        return m_generations_count;
    }

    void get_next_dna(bool update_number = true) {
        if (update_number) {
            ++m_current_dna;
        }
        std::ofstream f;
        size_t iters = m_generation[m_current_dna].dna.size();
        f.open("/home/tekatod/develop/cppstrategy2016/current.txt", std::ios::trunc);
        for(size_t i = 0; i < iters; ++i) {
            if(i != iters - 1) {
                f << m_generation[m_current_dna].dna[i] << std::endl;
            }
            else {
                f << m_generation[m_current_dna].dna[i];
            }
        }
        f.close();
    }

    void set_current_score(double score) {
        m_generation[m_current_dna].score = score;
    }

private:

    void write_best() {
        m_best_log << "generation number: " << m_generation_number << " score: "
                   << m_generation[0].score << " factors: ";
        for(size_t i = 0; i < m_generation[0].dna.size(); ++i) {
            m_best_log << m_generation[0].dna[i] << " ";
        }
        m_best_log << std::endl;
    }

    void reproduce() {
        std::sort(m_generation.begin(), m_generation.end(), [](const creature& c1, const creature& c2) {
            return c1.score > c2.score;
        });
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> distribution(0, m_generation_size / 2);
        for(size_t i = (m_generation_size / 2); i < m_generation_size; ++i) {
            size_t mother = distribution(gen);
            size_t father = distribution(gen);
            //TODO: this can be optimized
            for (size_t j = 0; j < m_dna_size / 2; ++j) {
                m_generation[i].dna[j] = m_generation[mother].dna[j];
            }
            for(size_t j = m_dna_size / 2; j < m_dna_size; ++j) {
                m_generation[i].dna[j] = m_generation[father].dna[j];
            }
        }
    }

    void mutate() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> distribution(0, m_generation_size - 1);
        for (size_t i = 0; i < m_mutants_count; ++i) {
            size_t mutant = distribution(gen);
            std::random_device rd1;
            std::mt19937 gen1(rd());
            std::uniform_int_distribution<size_t> distribution1(1, m_dna_size);
            std::random_device rd2;
            std::mt19937 gen2(rd());
            std::uniform_real_distribution<> distribution2(0.0, m_dna_size * 2);
            size_t gens_to_mutate = distribution1(gen1);
            std::random_device rd3;
            std::mt19937 gen3(rd());
            std::uniform_int_distribution<size_t> distribution3(0, m_dna_size - 1);
            for (size_t j = 0; j < gens_to_mutate; ++j) {
                m_generation[mutant].dna[distribution3(gen3)] = distribution2(gen2);
            }
        }
    }

    struct creature {
        std::vector<double> dna;
        double score;
    };
    std::vector<creature> m_generation;
    size_t m_generations_count;
    size_t m_dna_size;
    size_t m_generation_size;
    double m_mutants_count;
    size_t m_current_dna;
    size_t m_generation_number;
    std::ofstream m_best_log;
};