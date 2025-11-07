-- ===================================================================
--            CERVEAU DU BOSS "GUARDIAN" - VERSION 3.0 (État Persistant)
-- ===================================================================

local CONFIG = {
    -- Seuils et Limites
    PHASE_2_HP_THRESHOLD = 500,  -- La vie à laquelle il s'énerve
    X_BOUNDARY           = 900,   -- La limite gauche de sa zone de patrouille
    
    -- Paramètres de la Phase 1 (Calme)
    PHASE1 = {
        HORIZONTAL_SPEED   = 40.0,
        AMPLITUDE          = 150.0, -- Ampleur de l'oscillation verticale
        FREQUENCY          = 0.5,   -- Vitesse de l'oscillation
        ATTACK_COOLDOWN    = 4.0,   -- Temps entre chaque attaque
        BULLET_COUNT       = 12,    -- Nombre de balles dans le cercle
        BULLET_SPEED       = 150
    },

    -- Paramètres de la Phase 2 (Enragé)
    PHASE2 = {
        SCALE              = 4.0,   -- À quelle point il grossit
        AMPLITUDE          = 300.0, -- Il balaye plus l'écran
        FREQUENCY          = 1.2,   -- Il oscille plus vite
        ATTACK_COOLDOWN    = 1.2,   -- Il tire beaucoup plus vite
        BULLET_SPEED       = 300
    }
}

-- self est maintenant une table Lua persistante.
-- Les appels au moteur se font via self.entity.
function init(self)
    self.phase = 1
    self.attack_timer = 4.0
    self.oscillation_timer = 0
    print("[BOSS SCRIPT] Cerveau initialisé. self.phase = " .. self.phase)
    
    local init_pos = self.entity:getPosition()
    local init_vel = self.entity:getVelocity()
    print("[BOSS SCRIPT - INIT] Position initiale: x=" .. init_pos.x .. ", y=" .. init_pos.y)
    print("[BOSS SCRIPT - INIT] Vélocité initiale: dx=" .. init_vel.dx .. ", dy=" .. init_vel.dy)
    io.stdout:flush()
end

function on_update(self, dt)
    print("[BOSS SCRIPT] on_update called with dt = " .. dt)
    print("[BOSS SCRIPT] Current phase = " .. self.phase)
    print("[BOSS SCRIPT] Oscillation timer = " .. self.oscillation_timer)
    io.stdout:flush()

    self.oscillation_timer = self.oscillation_timer + dt

    if self.phase == 1 then
        update_phase_1(self, dt)
    else
        update_phase_2(self, dt)
    end
end

function on_damage(self, amount)
    local current_hp = self.entity:get_health()
    print("[BOSS SCRIPT] Impact détecté! Vie: " .. current_hp)
    io.stdout:flush()

    if self.phase == 1 and current_hp < CONFIG.PHASE_2_HP_THRESHOLD then
        print("[BOSS SCRIPT] PASSAGE EN PHASE 2!")
        io.stdout:flush()
        
        self.phase = 2
        
        self.entity:setScale(CONFIG.PHASE2.SCALE, CONFIG.PHASE2.SCALE)
        self.entity:setVelocity(0, self.entity:get_velocity().dy)
        self.attack_timer = CONFIG.PHASE2.ATTACK_COOLDOWN
    end
end

function update_phase_1(self, dt)
    print("[BOSS SCRIPT] Phase 1 update called")
    io.stdout:flush()
    
    local current_vel = self.entity:get_velocity()
    print("[BOSS SCRIPT] Current velocity: dx=" .. current_vel.dx .. ", dy=" .. current_vel.dy)
    io.stdout:flush()
    
    local pos = self.entity:getPosition()
    print("[BOSS SCRIPT] Current position: x=" .. pos.x .. ", y=" .. pos.y)
    io.stdout:flush()
    
    local new_dx = current_vel.dx

    if (pos.x < 900 and new_dx < 0) or (pos.x > 1200 and new_dx > 0) then
        new_dx = -new_dx
    end
    
    local new_dy = math.cos(self.oscillation_timer * CONFIG.PHASE1.FREQUENCY) * CONFIG.PHASE1.AMPLITUDE
    self.entity:setVelocity(new_dx, new_dy)

    self.attack_timer = self.attack_timer - dt
    if self.attack_timer <= 0 then
        print("[BOSS SCRIPT - Phase 1] Attaque!")
        io.stdout:flush()
        fire_circle_pattern(self)
        self.attack_timer = CONFIG.PHASE1.ATTACK_COOLDOWN
    end
end

function update_phase_2(self, dt)
    local new_dy = math.cos(self.oscillation_timer * CONFIG.PHASE2.FREQUENCY) * CONFIG.PHASE2.AMPLITUDE
    self.entity:setVelocity(0, new_dy)

    self.attack_timer = self.attack_timer - dt
    if self.attack_timer <= 0 then
        print("[BOSS SCRIPT - Phase 2] Attaque!")
        io.stdout:flush()
        fire_aimed_shot(self)
        self.attack_timer = CONFIG.PHASE2.ATTACK_COOLDOWN
    end
end

function fire_circle_pattern(self)
    local pos = self.entity:getPosition()
    for i = 0, 11 do
        local angle = i * (2 * math.pi / 12)
        local dx = math.cos(angle) * CONFIG.PHASE1.BULLET_SPEED
        local dy = math.sin(angle) * CONFIG.PHASE1.BULLET_SPEED
        local bullet_id = engine.spawn_from_archetype("projectiles.enemy_default_bullet_charged", pos.x - 150, pos.y + 50)
        engine.set_velocity(bullet_id, dx, dy)
    end
end

function fire_aimed_shot(self)
    local boss_pos = self.entity:getPosition()
    local player_pos = engine.get_player_position()

    if not player_pos then return end

    local dir_x = player_pos.x - (boss_pos.x + 64)
    local dir_y = player_pos.y - (boss_pos.y + 64)
    local magnitude = math.sqrt(dir_x*dir_x + dir_y*dir_y)

    if magnitude > 0 then
        local dx = (dir_x / magnitude) * CONFIG.PHASE2.BULLET_SPEED
        local dy = (dir_y / magnitude) * CONFIG.PHASE2.BULLET_SPEED
        local bullet_id = engine.spawn_from_archetype("projectiles.simple_bullet_charged", boss_pos.x - 64, boss_pos.y + 100)
        engine.set_velocity(bullet_id, dx, dy)
    end
end