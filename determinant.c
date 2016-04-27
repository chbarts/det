typedef long double ld;

ld det2x2(ld a, ld b, ld c, ld d)
{
    return (a * d - b * c);
}

ld det3x3(ld a1, ld a2, ld a3, ld a4, ld a5, ld a6, ld a7, ld a8, ld a9)
{
    ld det1, det2, det3, det5;

    det1 = (a1 * det2x2(a5, a6, a8, a9));
    det2 = (a4 * det2x2(a2, a3, a8, a9));
    det3 = (a7 * det2x2(a2, a3, a5, a6));

    det5 = det1 - det2 + det3;

    return det5;
}
