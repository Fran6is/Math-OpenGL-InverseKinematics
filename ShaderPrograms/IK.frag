#version 430 

in vec2 TexCoord;

uniform vec2 UWindowDimension = vec2(0);
uniform vec2 UMousePosition   = vec2(0);


struct FBone
{
	vec2  Position;
	float Length;
	float Rotation;
	vec2  End;
	vec3  Color;
};

const FBone BoneDefault = FBone(vec2(0), 100, 0, vec2(0), vec3(1));

float GetRectangle( FBone Bone, vec2 Graph);

const int MAX_BONES = 20;

void ShowBones(  FBone Bones[MAX_BONES], int BonesCount, vec2 Graph )
{
	
	for(int i = 0; i < BonesCount; i++)
	{
		gl_FragColor.rgb += Bones[i].Color * GetRectangle( Bones[i], Graph );
	}

	
}

uniform float UGraphSize = 1000.0;
uniform FBone UBones[MAX_BONES];
uniform int   UTotalBones  = 0;
uniform float UBoneBreadth = 10.0;

float GetRectangle( FBone Bone, vec2 Graph);


void main(void)
{
	gl_FragColor = vec4(0);

	vec2 Graph = gl_FragCoord.xy / UWindowDimension;
	Graph = ( Graph - vec2(0.5) ) / 0.5;
	

	Graph    = Graph * (UGraphSize * 0.5);
	Graph.x *= UWindowDimension.x / UWindowDimension.y;


	ShowBones( UBones, UTotalBones, Graph );
}

float GetRectangle( FBone Bone, vec2 Graph)
{
	vec2  Position = Bone.Position;
	vec2  Size = vec2( Bone.Length, UBoneBreadth);
	float Rotation = Bone.Rotation;

	vec2 XBasis = vec2( cos( radians(Rotation) ), sin( radians(Rotation) ) );
	vec2 YBasis = vec2( -XBasis.y, XBasis.x );

	Position = vec2( dot(XBasis, Position), dot(YBasis, Position) );
	

	Graph = vec2( dot(XBasis, Graph ), dot(YBasis, Graph ) ) ;

	Graph = Graph - Position ;

	// float t = sign(Graph.y);
	// gl_FragColor.rgb += vec3( sign(Graph), 0.0 ) * 0.5;

	vec2 Rec = vec2(0);

	Rec.x  = Size.x - Graph.x; // positive 1 dimensional vector pointing to the positive side. So we need to force it to a negative and clamp
	Rec.x *= sign(Graph.x);  

	Rec.y = Size.y * 0.5 - abs(Graph.y);

	Rec = max(Rec, 0.0);

	float t = sign(Rec.x) ;

	// gl_FragColor.rgb += vec3(t) * 0.5;

	// t = sign(Rec.y) ;
	// gl_FragColor.rgb += vec3(t) * 0.5;

	//gl_FragColor.rgb += sign( Rec.x * Rec.y ) * 0.5;
	
	return sign( Rec.x * Rec.y );
}
