declare module '@radarlabs/s2' {

  export type ChildPosition = 0 | 1 | 2 | 3;

  export class LatLng {

    constructor(latDegrees: number, lngDegrees: number);

    public normalized(): LatLng;

    public latitude(): number;
    public longitude(): number;

    public toString(): string;

  }

  export class CellId {

    public constructor(id: string);
    public constructor(ll: LatLng);

    public id(): string;
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

  export class CellUnion {

    public constructor(tokens: string[]);
    public constructor(cellIds: CellId[]);

    public contains(cells: CellId | CellUnion): boolean;
    public intersects(cells: CellId | CellUnion): boolean;

    public union(cells: CellUnion): CellUnion;
    public intersection(cells: CellUnion): CellUnion;
    public difference(cells: CellUnion): CellUnion;

    public cellIds(): CellId[];
    public tokens(): string[];

  }

  export interface RegionCovererOptions {
    min?: number;
    max?: number;
    max_cells?: number;
  }

  export class RegionCoverer {
    public static getCoveringTokens(lls: LatLng[], options: RegionCovererOptions): string[] | null;
    public static getCovering(lls: LatLng[], options: RegionCovererOptions): CellUnion | null;
  }

}
