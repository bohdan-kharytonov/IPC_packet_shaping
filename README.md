# IPC_packet_shaping

## Package content 

| Header  | length  | Id     | Payload | Checksum |
|---------|---------|--------|---------|----------|
| uint_16 | uint_16 | uint_8 | 256     | uint_8   |

### Example of package 

```
0x01, 0x55,0x00, 0x0E, 0x2A, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x49, 0x50, 0x43, 0x21
```

#### Disassembling the package

- ```0x01 0x55``` - **Header** 
- ```0x00, 0x0E``` - **Length** 
- ```0x2A``` - **Id**
- ```0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x49, 0x50, 0x43``` - **Payload**
- ```0x21``` - **Checksum**

#### Output 

> Hello, IPC!
