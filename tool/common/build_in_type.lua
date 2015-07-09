BuildInTypeTable ={
	boolean =true,
	int8    =true,
	int16   =true,
	int32   =true,
	int64   =true,
	uint8   =true,
	uint16  =true,
	uint32  =true,
	uint64  =true,
	float32 =true,
	float64 =true,
	string  =true,
};
function is_build_in_type(t)
	return BuildInTypeTable[t]
end
