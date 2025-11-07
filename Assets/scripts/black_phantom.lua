-- Black Phantom Boss Script
-- Advanced multi-phase boss with dodge mechanics, duplication, and death explosion

-- Configuration
CONFIG = {
    -- Screen limits
    SCREEN_WIDTH = 1920,
    X_MIN_LIMIT = 960,  -- Boss cannot go past the middle of the screen (left half)
    
    -- Phase thresholds (based on HP percentage)
    PHASE_2_THRESHOLD = 0.66,  -- 2/3 HP remaining
    PHASE_3_THRESHOLD = 0.33,  -- 1/3 HP remaining
    
    -- Phase 1: Simple
    phase1 = {
        fire_rate = 1.2,
        projectile_type = "projectiles.black_phantom_projectile_simple",
        charged_shot_chance = 0.15,
        movement_speed = 30,
        pattern_change_time = 3.0
    },
    
    -- Phase 2: Serious
    phase2 = {
        fire_rate = 0.8,
        projectile_type = "projectiles.black_phantom_projectile_simple",
        charged_shot_chance = 0.25,
        dodge_chance = 0.70,  -- 70% chance to dodge simple bullets
        projectile_scale = 2.0,  -- 2x larger projectiles
        movement_speed = 50,
        pattern_change_time = 2.0
    },
    
    -- Phase 3: Enraged
    phase3 = {
        fire_rate = 0.5,
        projectile_type = "projectiles.black_phantom_projectile_charged",
        charged_shot_chance = 0.8,
        auto_dodge = true,  -- Automatically dodges all non-charged attacks
        immune_to_simple = true,  -- Takes no damage from simple bullets
        erratic_movement = true,
        duplication_on_charged_hit = true,
        movement_speed = 80,
        pattern_change_time = 1.5
    },
    
    -- Death explosion
    explosion = {
        radius = 500,
        damage = 100,
        effect_duration = 0.5
    }
}

-- State variables
state = {
    current_phase = 1,
    time_elapsed = 0,
    last_fire_time = 0,
    pattern_timer = 0,
    current_pattern = 0,
    dodge_cooldown = 0,
    has_duplicated = false,
    death_explosion_triggered = false,
    
    -- HP tracking
    max_hp = 0,
    
    -- Movement state
    movement_direction = 1,  -- 1 or -1
    base_y = 0,
    target_y = 0
}

-- Initialize boss
function init(self)
    print("Black Phantom boss initialized!")
    
    local pos = self.entity:getPosition()
    state.base_y = pos.y
    state.target_y = pos.y
    
    -- Store initial max HP
    local current_hp = self.entity:get_health()
    state.max_hp = current_hp
    print("Black Phantom max HP: " .. state.max_hp)
    
    -- Start in Phase 1
    state.current_phase = 1
    print("Phase 1: Simple - Starting combat")
end

-- Main update loop
function on_update(self, dt)
    state.time_elapsed = state.time_elapsed + dt
    state.pattern_timer = state.pattern_timer + dt
    state.dodge_cooldown = math.max(0, state.dodge_cooldown - dt)
    
    -- Check phase transitions
    check_phase_transition(self)
    
    -- Get current phase config
    local phase_config = get_current_phase_config()
    
    -- Update movement
    update_movement(self, dt, phase_config)
    
    -- Fire projectiles
    update_combat(self, dt, phase_config)
    
    -- Phase 2+: Check for incoming projectiles to dodge
    if state.current_phase >= 2 then
        check_dodge_opportunities(self, phase_config)
    end
end

-- Called when boss takes damage
function on_damage(self, damage_amount, projectile_type)
    print("Black Phantom took " .. damage_amount .. " damage!")
    
    -- Phase 3: Immune to simple bullets
    if state.current_phase == 3 and CONFIG.phase3.immune_to_simple then
        if projectile_type == "simple" or projectile_type == nil then
            print("Phase 3: Immune to simple damage!")
            -- Restore the HP that was taken (hackish, but works with the damage system)
            -- Note: This won't actually work as we can't modify HP from Lua
            -- Instead, we'll handle this in the collision system with a special check
            return
        end
    end
    
    -- Phase 3: Duplication on charged hit
    if state.current_phase == 3 and CONFIG.phase3.duplication_on_charged_hit then
        if projectile_type == "charged" and not state.has_duplicated then
            duplicate_boss(self)
            state.has_duplicated = true
        end
    end
    
    local current_hp = self.entity:get_health()
    print("Black Phantom HP: " .. current_hp .. "/" .. state.max_hp)
    
    -- Check if boss died
    if current_hp <= 0 and not state.death_explosion_triggered then
        trigger_death_explosion(self)
    end
end

-- Check if we should transition to next phase
function check_phase_transition(self)
    local current_hp = self.entity:get_health()
    local hp_percent = current_hp / state.max_hp
    
    if state.current_phase == 1 and hp_percent <= CONFIG.PHASE_2_THRESHOLD then
        enter_phase_2(self)
    elseif state.current_phase == 2 and hp_percent <= CONFIG.PHASE_3_THRESHOLD then
        enter_phase_3(self)
    end
end

-- Enter Phase 2: Serious
function enter_phase_2(self)
    state.current_phase = 2
    print("=== PHASE 2: SERIOUS MODE ACTIVATED ===")
    print("Boss now dodges simple projectiles and fires larger shots!")
    
    -- Visual feedback: significantly increase scale (from 2.5 to 3.5)
    self.entity:setScale(3.5, 3.5)
    print("Boss scale increased to 3.5x")
end

-- Enter Phase 3: Enraged
function enter_phase_3(self)
    state.current_phase = 3
    print("=== PHASE 3: ENRAGED MODE - ULTIMATE FORM ===")
    print("Boss is now immune to simple damage and moves erratically!")
    
    -- Visual feedback: massive scale increase (from 3.5 to 5.0)
    self.entity:setScale(5.0, 5.0)
    print("Boss scale increased to 5.0x - MASSIVE FORM!")
end

-- Get configuration for current phase
function get_current_phase_config()
    if state.current_phase == 1 then
        return CONFIG.phase1
    elseif state.current_phase == 2 then
        return CONFIG.phase2
    else
        return CONFIG.phase3
    end
end

-- Update boss movement
function update_movement(self, dt, phase_config)
    local pos = self.entity:getPosition()
    local vel = self.entity:getVelocity()
    
    -- Phase 3: Erratic movement
    if phase_config.erratic_movement then
        -- Random direction changes
        if state.pattern_timer >= phase_config.pattern_change_time then
            state.pattern_timer = 0
            state.movement_direction = math.random(-1, 1)
            state.target_y = state.base_y + math.random(-150, 150)
        end
        
        -- Move towards target Y with high speed
        local diff_y = state.target_y - pos.y
        if math.abs(diff_y) > 5 then
            vel.dy = diff_y * 2.0  -- Fast pursuit
        else
            vel.dy = 0
        end
    else
        -- Normal sinusoidal movement
        if state.pattern_timer >= phase_config.pattern_change_time then
            state.pattern_timer = 0
            state.movement_direction = state.movement_direction * -1
        end
        
        local amplitude = 100
        local frequency = 1.5
        local target_y = state.base_y + math.sin(state.time_elapsed * frequency) * amplitude
        
        local diff_y = target_y - pos.y
        vel.dy = diff_y * 1.5
    end
    
    -- Keep horizontal velocity (moving left)
    vel.dx = -phase_config.movement_speed
    
    -- Limit: Boss cannot go past the middle of the screen (X limit)
    if pos.x <= CONFIG.X_MIN_LIMIT then
        -- Stop moving left, can only move right or stay
        vel.dx = math.max(0, vel.dx)
        print("Black Phantom reached X limit at " .. pos.x)
    end
    
    self.entity:setVelocity(vel.dx, vel.dy)
end

-- Combat logic
function update_combat(self, dt, phase_config)
    -- ATTAQUES DÉSACTIVÉES POUR LES TESTS
    -- local time_since_last_fire = state.time_elapsed - state.last_fire_time
    
    -- if time_since_last_fire >= phase_config.fire_rate then
    --     state.last_fire_time = state.time_elapsed
    --     
    --     -- Decide between simple and charged shot
    --     local use_charged = math.random() < phase_config.charged_shot_chance
    --     local projectile_type = use_charged and "projectiles.black_phantom_projectile_charged" or phase_config.projectile_type
    --     
    --     -- Fire pattern based on phase
    --     if state.current_phase == 1 then
    --         fire_simple_pattern(self, projectile_type)
    --     elseif state.current_phase == 2 then
    --         fire_serious_pattern(self, projectile_type, phase_config.projectile_scale)
    --     else
    --         fire_enraged_pattern(self, projectile_type)
    --     end
    -- end
end

-- Phase 1: Simple attack patterns
function fire_simple_pattern(self, projectile_type)
    local pos = self.entity:getPosition()
    
    -- Alternate between aimed shot and spread
    if state.current_pattern % 2 == 0 then
        -- Single aimed shot
        fire_aimed_shot(self, projectile_type, 0, 0)
    else
        -- 3-way spread
        for i = -1, 1 do
            fire_aimed_shot(self, projectile_type, 0, i * 80)
        end
    end
    
    state.current_pattern = state.current_pattern + 1
end

-- Phase 2: Serious attack patterns
function fire_serious_pattern(self, projectile_type, scale)
    local pos = self.entity:getPosition()
    
    -- 5-way spread with scaled projectiles
    for i = -2, 2 do
        local bullet = fire_aimed_shot(self, projectile_type, 0, i * 60)
        
        -- Scale up projectiles if bullet was successfully spawned
        if bullet ~= nil and scale > 1.0 then
            -- Note: We need to add a method to scale spawned entities
            -- For now, projectiles will use their default size
        end
    end
end

-- Phase 3: Enraged attack patterns
function fire_enraged_pattern(self, projectile_type)
    local pos = self.entity:getPosition()
    
    -- Chaotic burst - 8 projectiles in a circle plus aimed shot
    for i = 0, 7 do
        local angle = (i / 8) * 2 * math.pi
        local vel_x = math.cos(angle) * -300
        local vel_y = math.sin(angle) * 300
        
        spawn_projectile(self, projectile_type, 0, 0, vel_x, vel_y)
    end
    
    -- Extra aimed shot at player
    fire_aimed_shot(self, projectile_type, 0, 0)
end

-- Fire a projectile aimed at player
function fire_aimed_shot(self, projectile_type, offset_x, offset_y)
    local pos = self.entity:getPosition()
    
    -- Get player position using engine API (like boss_guardian.lua)
    local player_pos = engine.get_player_position()
    
    if not player_pos then
        print("Warning: Could not get player position, using default direction")
        -- Default to shooting left if player position not available
        return spawn_projectile(self, projectile_type, offset_x, offset_y, -300, 0)
    end
    
    -- Calculate direction to player
    local dx = player_pos.x - (pos.x + offset_x)
    local dy = player_pos.y - (pos.y + offset_y)
    local distance = math.sqrt(dx * dx + dy * dy)
    
    -- Normalize and scale to projectile speed
    local speed = 300
    local vel_x = (dx / distance) * speed
    local vel_y = (dy / distance) * speed
    
    return spawn_projectile(self, projectile_type, offset_x, offset_y, vel_x, vel_y)
end

-- Spawn a projectile
function spawn_projectile(self, projectile_type, offset_x, offset_y, vel_x, vel_y)
    local pos = self.entity:getPosition()
    
    -- Spawn the projectile at the specified position (like boss_guardian.lua)
    local bullet_id = engine.spawn_from_archetype(
        projectile_type,
        pos.x + offset_x,
        pos.y + offset_y
    )
    
    -- Set the velocity separately using engine.set_velocity
    if bullet_id then
        engine.set_velocity(bullet_id, vel_x, vel_y)
    end
    
    return bullet_id
end

-- Phase 2+: Check for incoming player projectiles and dodge
function check_dodge_opportunities(self, phase_config)
    -- This is a placeholder - actual dodge detection would require:
    -- 1. Access to all entities in the scene
    -- 2. Filtering for player projectiles (MissileTag owner="PLAYER")
    -- 3. Checking if any are within dodge range
    -- 4. Executing dodge movement
    
    -- For now, we'll simulate random dodge behavior
    if state.dodge_cooldown <= 0 then
        local should_dodge = false
        
        if state.current_phase == 3 and phase_config.auto_dodge then
            -- Phase 3: Always dodge (simulated with high chance)
            should_dodge = math.random() < 0.3  -- 30% chance per frame
        elseif phase_config.dodge_chance and math.random() < 0.01 then  -- 1% chance per frame
            should_dodge = true
        end
        
        if should_dodge then
            execute_dodge(self)
            state.dodge_cooldown = 1.0  -- 1 second cooldown
        end
    end
end

-- Execute a dodge movement
function execute_dodge(self)
    print("Black Phantom dodges!")
    
    local vel = self.entity:getVelocity()
    
    -- Quick burst movement
    local dodge_direction = math.random() < 0.5 and 1 or -1
    vel.dy = dodge_direction * 400  -- Fast vertical movement
    
    self.entity:setVelocity(vel.dx, vel.dy)
end

-- Duplicate the boss (Phase 3)
function duplicate_boss(self)
    print("=== Black Phantom DUPLICATES! ===")
    
    local pos = self.entity:getPosition()
    
    -- Spawn a duplicate boss offset from original position
    -- Note: The duplicate will have full HP and be in Phase 1
    local duplicate_id = engine.spawn_from_archetype(
        "bosses.black_phantom",
        pos.x + 150,
        pos.y + 100
    )
    
    -- Set initial velocity for the duplicate (stationary or slight movement)
    if duplicate_id then
        engine.set_velocity(duplicate_id, 0, 0)
        print("Duplicate boss spawned with ID: " .. duplicate_id)
    else
        print("Warning: Failed to spawn duplicate boss")
    end
end

-- Trigger death explosion
function trigger_death_explosion(self)
    state.death_explosion_triggered = true
    print("=== Black Phantom DEFEATED - MASSIVE EXPLOSION! ===")
    
    local pos = self.entity:getPosition()
    
    -- Note: Actual explosion damage to player would need to be implemented
    -- in the collision system by checking distance to boss on death
    -- For now, we'll just spawn multiple explosion visual effects in a circle pattern
    for i = 0, 15 do
        local angle = (i / 16) * 2 * math.pi
        local offset_x = math.cos(angle) * CONFIG.explosion.radius * 0.5
        local offset_y = math.sin(angle) * CONFIG.explosion.radius * 0.5
        
        -- Spawn explosion particles using the new archetype
        local explosion_id = engine.spawn_from_archetype(
            "projectiles.explosion_particle",
            pos.x + offset_x,
            pos.y + offset_y
        )
        
        if explosion_id then
            -- Set slight outward velocity for visual effect
            local vel_x = math.cos(angle) * 50
            local vel_y = math.sin(angle) * 50
            engine.set_velocity(explosion_id, vel_x, vel_y)
        end
    end
    
    -- Spawn a central large explosion
    local center_explosion = engine.spawn_from_archetype(
        "projectiles.explosion_particle",
        pos.x,
        pos.y
    )
    
    if center_explosion then
        engine.set_velocity(center_explosion, 0, 0)
        print("Central explosion spawned at boss position")
    end
    
    print("Explosion radius: " .. CONFIG.explosion.radius .. " - Damage: " .. CONFIG.explosion.damage)
end
