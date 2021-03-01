bool testEmergence(){
    return  0;
};

void scenario_STOP()
{
    desiredState = 0; //DEGUB
    // //Valves
    actuator1->off();
    actuator2->off();
    actuator3->off();
    actuator4->off();
    // Desactivo motor piscina
    actuator5->off();
    // Desactivo motor intercambiador
    actuator7->off();
    // Desactivo motor suelo
    actuator6->off();
};

void scenario_pool()
{
    desiredState = 10; //DEGUB
    // //Valves
    actuator1->off();
    actuator2->off();
    actuator3->off();
    actuator4->off();
    // Desactivo motor suelo
    actuator6->off();
    // Control
    // if (desiredTempPool > )
    // Activo motor piscina
    actuator5->on();
    // Activo motor intercambiador
    actuator7->on();
};

void scenario_floor()
{
    desiredState = 20; //DEGUB
    // //Valves
    actuator1->on();
    actuator2->off();
    actuator3->on();
    actuator4->off();
    // Desactivo motor piscina
    actuator5->off();
    // Desactivo motor intercambiador
    actuator7->off();
    // Activo motor suelo
    actuator6->on();
};

void scenario_hot_paralell()
{
    desiredState = 30; //DEGUB
    // //Valves
    actuator1->off();
    actuator2->off();
    actuator3->on();
    actuator4->off();
    // Activo motor piscina
    actuator5->on();
    // Activo motor intercambiador
    actuator7->on();
    // Activo motor suelo
    actuator6->on();
};

void scenario_hot_series()
{
    desiredState = 40; //DEGUB
    // //Valves
    actuator1->off();
    actuator2->on();
    actuator3->on();
    actuator4->off();
    // Activo motor piscina
    actuator5->on();
    // Activo motor intercambiador
    actuator7->on();
    // Activo motor suelo
    actuator6->on();
};

void scenario_cold_paralell()
{
    desiredState = 50; //DEGUB
    // //Valves
    actuator1->off();
    actuator2->off();
    actuator3->on();
    actuator4->on();
    // Activo motor piscina
    actuator5->on();
    // Activo motor intercambiador
    actuator7->on();
    // Activo motor suelo
    actuator6->on();
};

void scenario_cold_series()
{
    desiredState = 60; //DEGUB
    //Valves
    actuator1->off();
    actuator2->on();
    actuator3->off();
    actuator4->on();
    // Activo motor piscina
    actuator5->on();
    // Activo motor intercambiador
    actuator7->on();
    // Activo motor suelo
    actuator6->on();
};

void updateScenario(int desiredState){
    switch (desiredState)
    {
    case 10:
        testEmergence();
        scenario_pool();
        break;
    case 20:
        testEmergence();
        scenario_floor();
        break;
    case 30:
        testEmergence();
        scenario_hot_paralell();
        break;
    case 40:
        testEmergence();
        scenario_hot_series();
        break;
    case 50:
        testEmergence();
        void scenario_cold_paralell();
        break;
    case 60:
        testEmergence();
        scenario_cold_series();
        break;   
    default:
        scenario_STOP();
        break;
    }

}