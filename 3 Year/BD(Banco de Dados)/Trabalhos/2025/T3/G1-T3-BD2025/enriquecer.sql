/* Inserindo 5 Aeroportos */
INSERT INTO AEROPORTO (NOME_AEROPORTO, CIDADE, ESTADO) VALUES
('Aeroporto de Congonhas', 'São Paulo', 'SP'),
('Aeroporto de Guarulhos', 'Guarulhos', 'SP'),
('Aeroporto Santos Dumont', 'Rio de Janeiro', 'RJ'),
('Aeroporto de Campo Grande', 'Campo Grande', 'MS'),
('Aeroporto de Brasília', 'Brasília', 'DF');

/* Inserindo 5 Tipos de Aviao */
INSERT INTO TIPO_AVIAO (NOME_TIPO, NUMERO_MAXIMO_ASSENTOS, EMPRESA_FABRICANTE) VALUES
('Boeing 737', 180, 'Boeing'),
('Airbus A320', 170, 'Airbus'),
('Embraer E195', 124, 'Embraer'),
('ATR 72', 70, 'ATR'),
('Boeing 777', 300, 'Boeing');

/* Ligando quais avioes podem pousar em quais aeroportos */
/* (Todos podem pousar em todos - para simplificar) */
INSERT INTO POUSA (CODIGO_AEROPORTO, NOME_TIPO) VALUES
(1, 'Boeing 737'),
(1, 'Airbus A320'),
(2, 'Boeing 777'),
(3, 'Embraer E195'),
(4, 'ATR 72');

/* Inserindo 5 Avioes */
INSERT INTO AVIAO (PREFIXO, NUMERO_TOTAL_ASSENTOS, NOME_TIPO) VALUES
('PR-GOL', 180, 'Boeing 737'),
('PR-LAT', 170, 'Airbus A320'),
('PR-AZU', 124, 'Embraer E195'),
('PP-VOE', 70, 'ATR 72'),
('PT-TAM', 300, 'Boeing 777');

/* Inserindo 5 Voos */
INSERT INTO VOO (EMPRESA) VALUES
('GOL'),
('LATAM'),
('AZUL'),
('GOL'),
('LATAM');

/* Inserindo dias da semana para os voos (1=Seg, 3=Qua, 5=Sex) */
INSERT INTO DIAS_SEMANAS_VOO (NUMERO_VOO, DIA_SEMANA) VALUES
(1, 1), /* Voo 1 na Segunda */
(1, 3), /* Voo 1 na Quarta */
(2, 2), /* Voo 2 na Terça */
(3, 5), /* Voo 3 na Sexta */
(5, 1); /* Voo 5 na Segunda */

/* Inserindo 5 Segmentos de Voo  */
/* (Aeroportos: 1=CGH, 2=GRU, 3=SDU, 4=CGR, 5=BSB) */
INSERT INTO SEGMENTO_VOO (NUMERO_VOO, NUMERO_SEGMENTO, CODIGO_AEROPORTO_PARTIDA, CODIGO_AEROPORTO_CHEGADA, HORARIO_PREVISTO_PARTIDA, HORARIO_PREVISTO_CHEGADA, PREFIXO) VALUES
(1, 1, 1, 3, '08:00:00', '09:00:00', 'PR-GOL'), /* Voo 1 (GOL): CGH -> SDU */
(1, 2, 3, 4, '09:30:00', '11:00:00', 'PR-GOL'), /* Voo 1 (GOL): SDU -> CGR */
(2, 1, 2, 5, '10:00:00', '12:00:00', 'PR-LAT'), /* Voo 2 (LATAM): GRU -> BSB */
(3, 1, 1, 4, '14:00:00', '15:30:00', 'PR-AZU'), /* Voo 3 (AZUL): CGH -> CGR */
(5, 1, 2, 3, '07:00:00', '08:00:00', 'PT-TAM'); /* Voo 5 (LATAM): GRU -> SDU */

/* Inserindo 5 Instancias de Segmento*/
INSERT INTO INSTANCIA_DE_SEGMENTO (NUMERO_VOO, NUMERO_SEGMENTO, DATA_INSTANCIA, HORARIO_PARTIDA_AEROPORTO, HORARIO_CHEGADA_AEROPORTO, NUMERO_ASSENTOS_DISPONIVEIS) VALUES
(1, 1, '2025-11-03', '08:05:00', '09:10:00', 50), /* Voo 1, Seg 1 (Segunda) */
(1, 2, '2025-11-03', '09:35:00', '11:05:00', 30), /* Voo 1, Seg 2 (Segunda) */
(2, 1, '2025-11-04', '10:00:00', '12:15:00', 100),/* Voo 2, Seg 1 (Terça) */
(3, 1, '2025-11-07', '14:10:00', '15:40:00', 20), /* Voo 3, Seg 1 (Sexta) */
(5, 1, '2025-11-10', '07:00:00', '08:00:00', 150);/* Voo 5, Seg 1 (Segunda) */
