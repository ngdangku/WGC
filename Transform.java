/* Transform.java
   Copyright (C) 2007, Francisco Claude
   Copyright (C) 2010, Sebastiano Vigna
   Copyright (C) 2020, Dengke Wu

   Transform

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.


 */

import it.unimi.dsi.webgraph.BVGraph;
import it.unimi.dsi.webgraph.NodeIterator;

import java.io.DataOutputStream;

class Transform {

	public static void main( String[] args ) throws Exception {
		if ( args.length != 1 ) {
			System.out.println( "Usage: Transform <basename>" );
			System.out.println( "Reads a graph with given basename in WebGraph format and writes it to standard output in RP binary format." );
			return;
		}

		final BVGraph g = BVGraph.loadOffline( args[ 0 ] );
		DataOutputStream out = new DataOutputStream( System.out );
		out.writeInt( Integer.reverseBytes( g.numNodes() ) );
		out.writeLong( Long.reverseBytes( g.numArcs() ) );
		final int n = g.numNodes();
		final NodeIterator nodeIterator = g.nodeIterator();
		for ( int i = 0; i < n; i++ ) {
			nodeIterator.nextInt();
			final int[] succ = nodeIterator.successorArray();
			final int d = nodeIterator.outdegree();
			out.writeInt( Integer.reverseBytes( -( i + 1 ) ) );
			for ( int j = 0; j < d; j++ ) out.writeInt( Integer.reverseBytes( 1 + succ[ j ] ) );
		}
		out.flush();
	}

}
