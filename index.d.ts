declare module '@radarlabs/s2' {

  export type ChildPosition = 0 | 1 | 2 | 3;

  export class Point {

    constructor(x: number, y: number, z: number);

    public x(): number;
    public y(): number;
    public z(): number;
  }

  export class LatLng {

    constructor(latDegrees: number, lngDegrees: number);
    constructor(point: Point);

    public normalized(): LatLng;

    public latitude(): number;
    public longitude(): number;

    public toString(): string;
  }

  export class CellId {

    public constructor(id: bigint);
    public constructor(token: string);
    public constructor(ll: LatLng);

    public id(): bigint;
    public idString(): string;
    public token(): string;

    public contains(other: CellId): boolean;
    public intersects(other: CellId): boolean;
    public isLeaf(): boolean;

    public parent(level?: number): CellId;
    public child(position: ChildPosition): CellId;
    public next(): CellId;

    public level(): number;

    public static fromToken(token: string): CellId;
  }

  export class Cell {
    public constructor(cellId: CellId);

    public getVertex(pos: number): Point;
    public getCenter(): Point;
  }

  export class CellUnion {

    public constructor(tokens: string[]);
    public constructor(cellIds: CellId[]);

    public contains(cells: CellId | CellUnion): boolean;
    public intersects(cells: CellId | CellUnion): boolean;

    public union(cells: CellUnion): CellUnion;
    public intersection(cells: CellUnion): CellUnion;
    public difference(cells: CellUnion): CellUnion;

    public ids(): BigUint64Array;
    public cellIds(): CellId[];
    public tokens(): string[];
  }

  export interface RegionCovererOptions {
    min?: number;
    max?: number;
    max_cells?: number;
  }

  export class RegionCoverer {
    public static getCoveringIds(lls: LatLng[], options: RegionCovererOptions): BigUint64Array | null;
    public static getCoveringTokens(lls: LatLng[], options: RegionCovererOptions): string[] | null;
    public static getCovering(lls: LatLng[], options: RegionCovererOptions): CellUnion | null;

    public static getRadiusCoveringIds(ll: LatLng, radiusM: number, options: RegionCovererOptions): BigUint64Array | null;
    public static getRadiusCoveringTokens(ll: LatLng, radiusM: number, options: RegionCovererOptions): string[] | null;
    public static getRadiusCovering(ll: LatLng, radiusM: number, options: RegionCovererOptions): CellUnion | null;
  }
}
