void offImpPool()
{
    int _curentTempPoolImp = sensor3->printValueIntx10();
    if (safetyTempPoolPVC < _curentTempPoolImp)
    {
        actuator5->on(); // Motor piscina
    }
    else
    {
        actuator5->off(); // Motor piscina
    }
}

void scenario_STOP()
{
    desiredState = 0;
    //Enable outputs
    actuator8->on(); //It goes the other way arround
    // //Valves
    actuator1->off();
    actuator2->off();
    actuator3->off();
    actuator4->off();
    // Desactivo motor piscina
    offImpPool();
    // Desactivo motor intercambiador
    actuator7->off();
    // Desactivo motor suelo
    actuator6->off();
};

void scenario_pool()
{
    desiredState = 10;
    int _curentTempPool = sensor2->printValueIntx10();
    int _curentTempPoolImp = sensor3->printValueIntx10();
    //Enable outputs
    actuator8->off(); //It goes the other way arround
    // //Valves
    actuator1->off(); //vk1
    actuator2->off(); //vk2
    actuator3->off(); //vk3
    actuator4->off(); //vk4
    actuator6->off(); // Motor suelo

    if (_curentTempPoolImp > alarmHighTempImp)
    {
        actuator5->on();  // Motor piscina
        actuator7->off(); // Motor intercambiador
        alarmState = true;
    }
    else
    {
        if (_curentTempPool > (desiredTempPool + hysteresisTempPool / 2))
        {
            actuator7->off(); // Motor intercambiador
            offImpPool();     // Motor piscina
        }
        if (_curentTempPool < (desiredTempPool - hysteresisTempPool / 2))
        {
            actuator5->on(); // Motor piscina
            actuator7->on(); // Motor intercambiador
        }
    }
};

void scenario_floor()
{
    desiredState = 20;
    int _curentTempFloor = sensor9->printValueIntx10();
    //Enable outputs
    actuator8->off(); //It goes the other way arround
    // //Valves
    actuator1->on();  //vk1
    actuator2->off(); //vk2
    actuator3->on();  //vk3
    actuator4->off(); //vk4
    offImpPool();     // Motor piscina
    actuator7->off(); // Motor intercambiador

    if (_curentTempFloor > (desiredTempFloor + hysteresisTempFloor / 2))
    {
        actuator6->off(); // Motor suelo
    }
    if (_curentTempFloor < (desiredTempFloor - hysteresisTempFloor / 2))
    {
        actuator6->on(); // Motor suelo
    }
};

void scenario_hot_series()
{
    desiredState = 30;
    int _curentTempPool = sensor2->printValueIntx10();
    int _curentTempPoolImp = sensor3->printValueIntx10();
    //Enable outputs
    actuator8->off(); //It goes the other way arround
    // //Valves
    actuator1->off();
    actuator2->on();
    actuator3->on();
    actuator4->off();
    //Motores
    if (_curentTempPoolImp > alarmHighTempImp)
    {
        actuator5->on();  // Motor piscina
        actuator7->off(); // Motor intercambiador
        actuator6->off(); // Motor suelo
        alarmState = true;
    }
    else
    {
        if (_curentTempPool > (desiredTempPool + hysteresisTempPool / 2))
        {
            actuator7->off(); // Motor intercambiador
            actuator6->off(); // Motor suelo
            offImpPool();     // Motor piscina
        }
        if (_curentTempPool < (desiredTempPool - hysteresisTempPool / 2))
        {
            actuator5->on(); // Motor piscina
            actuator7->on(); // Motor intercambiador
            actuator6->on(); // Motor suelo
        }
    }
};

void scenario_hot_paralell()
{
    desiredState = 40;
    int _curentTempPool = sensor2->printValueIntx10();
    int _curentTempPoolImp = sensor3->printValueIntx10();
    int _curentTempFloor = sensor9->printValueIntx10();
    //Enable outputs
    actuator8->off(); //It goes the other way arround
    // //Valves
    actuator1->off(); //vk1
    actuator2->off(); //vk2
    actuator3->on();  //vk3
    actuator4->off(); //vk4
    // Piscina
    if (_curentTempPoolImp > alarmHighTempImp)
    {
        actuator5->on();  // Motor piscina
        actuator7->off(); // Motor intercambiador
        alarmState = true;
    }
    else
    {
        if (_curentTempPool > (desiredTempPool + hysteresisTempPool / 2))
        {
            actuator7->off(); // Motor intercambiador
            offImpPool();     // Motor piscina
        }
        if (_curentTempPool < (desiredTempPool - hysteresisTempPool / 2))
        {
            actuator5->on(); // Motor piscina
            actuator7->on(); // Motor intercambiador
        }
    }
    // Suelo
    if (_curentTempFloor > (desiredTempFloor + hysteresisTempFloor / 2))
    {
        actuator6->off(); // Motor suelo
    }
    if (_curentTempFloor < (desiredTempFloor - hysteresisTempFloor / 2))
    {
        actuator6->on(); // Motor suelo
    }
};

void scenario_cold_series()
{
    desiredState = 50;
    int _curentTempFloor = sensor9->printValueIntx10();
    int _curentTempPoolImp = sensor3->printValueIntx10();
    //Enable outputs
    actuator8->off(); //It goes the other way arround
    //Valves
    actuator1->off(); //vk1
    actuator2->on();  //vk2
    actuator3->off(); //vk3
    actuator4->on();  //vk4

    if (_curentTempPoolImp > alarmHighTempImp)
    {
        actuator5->on();  // Motor piscina
        actuator7->off(); // Motor intercambiador
        actuator6->off(); // Motor suelo
        alarmState = true;
    }
    else
    {
        if (_curentTempFloor > (desiredTempFloor + hysteresisTempFloor / 2))
        {
            actuator5->on(); // Motor piscina
            actuator7->on(); // Motor intercambiador
            actuator6->on(); // Motor suelo
        }
        if (_curentTempFloor < (desiredTempFloor - hysteresisTempFloor / 2))
        {
            offImpPool();     // Motor piscina
            actuator7->off(); // Motor intercambiador
            actuator6->off(); // Motor suelo
        }
    }
};

void scenario_cold_paralell()
{
    desiredState = 60;
    int _curentTempPoolImp = sensor3->printValueIntx10();
    int _curentTempFloor = sensor9->printValueIntx10();
    //Enable outputs
    actuator8->off(); //It goes the other way arround
    // //Valves
    actuator1->off();
    actuator2->off();
    actuator3->on();
    actuator4->on();
    //Motors
    if (_curentTempPoolImp > alarmHighTempImp)
    {
        actuator5->on();  // Motor piscina
        actuator7->off(); // Motor intercambiador
        actuator6->off(); // Motor suelo
        alarmState = true;
    }
    else
    {
        if (_curentTempFloor > (desiredTempFloor + hysteresisTempFloor / 2))
        {
            actuator5->on(); // Motor piscina
            actuator7->on(); // Motor intercambiador
            actuator6->on(); // Motor suelo
        }
        if (_curentTempFloor < (desiredTempFloor - hysteresisTempFloor / 2))
        {
            offImpPool();     // Motor piscina
            actuator7->off(); // Motor intercambiador
            actuator6->off(); // Motor suelo
        }
    }
};

bool testEmergence()
{
    if (sensor1->printValueIntx10() > alarmHighTemp)
    {
        scenario_STOP();
        alarmState = true;
        return 1;
    }
    if (sensor2->printValueIntx10() > alarmHighTemp)
    {
        scenario_STOP();
        alarmState = true;
        return 1;
    }
    if (sensor3->printValueIntx10() > alarmHighTemp)
    {
        scenario_STOP();
        alarmState = true;
        return 1;
    }
    if (sensor4->printValueIntx10() > alarmHighTemp)
    {
        scenario_STOP();
        alarmState = true;
        return 1;
    }
    if (sensor5->printValueIntx10() > alarmHighTemp)
    {
        scenario_STOP();
        alarmState = true;
        return 1;
    }
    if (sensor6->printValueIntx10() > alarmHighTemp)
    {
        scenario_STOP();
        alarmState = true;
        return 1;
    }
    //NO alarms
    alarmState = false;
    return 0;
};

void updateScenario(int desiredState)
{
    switch (desiredState)
    {
    case 10:
        if (testEmergence())
        {
            break;
        };
        scenario_pool();
        break;
    case 20:
        if (testEmergence())
        {
            break;
        };
        scenario_floor();
        break;
    case 30:
        if (testEmergence())
        {
            break;
        };
        scenario_hot_series();
        break;
    case 40:
        if (testEmergence())
        {
            break;
        };
        scenario_hot_paralell();
        break;
    case 50:
        if (testEmergence())
        {
            break;
        };
        scenario_cold_series();
        break;
    case 60:
        if (testEmergence())
        {
            break;
        };
        scenario_cold_paralell();

        break;
    default:
        scenario_STOP();
        break;
    }
}
