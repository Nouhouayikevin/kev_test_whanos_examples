/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** INetwork.hpp
*/

#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "../../../Client/include/Packet.hpp"
#include <boost/system/error_code.hpp>

class INetwork {
public:
    virtual ~INetwork() = default;

    /**
     * @brief Démarre le client réseau et commence à écouter.
     * @return true si le démarrage réussit, false sinon.
     */
    virtual bool start() = 0;

    /**
     * @brief Arrête le client réseau et ferme la connexion.
     * @return true si l'arrêt réussit, false sinon.
     */
    virtual bool stop() = 0;

    /**
     * @brief Envoie un paquet binaire au serveur.
     * @param data Les données à envoyer.
     * @return true si l’envoi est mis en file d'attente, false sinon.
     */
    virtual bool send(std::shared_ptr<std::vector<uint8_t>> message, const boost::system::error_code& error, std::size_t bytes_transferred) = 0;

    /**
     * @brief Vérifie s'il y a des paquets reçus en attente.
     * @return true s'il y a au moins un paquet, false sinon.
     */
    virtual bool receive() = 0;
    
    /**
     * @brief Récupère le plus ancien paquet reçu de la file d'attente.
     * @return Une paire contenant le header et le payload du paquet.
     */
    virtual std::pair<Packet_Header, std::vector<uint8_t>> getNextPacket() = 0;

    /**
     * @brief Vérifie si le client réseau est en cours d'exécution.
     * @return true si le client est actif, false sinon.
     */
    virtual bool isRunning() const = 0;
};