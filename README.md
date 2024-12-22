# succinct-local-hash
An attempt to build Hash functions with succinct local openings using SHA256 and Merkle Trees. An opening is a tuple $(j, d, op)$ where $d$ is the data at location $j$ and $op$ is a list of hashes that lie on the path from the root node to location $j$ in the Merkle Tree. Ideally, we expect the following properties:
- **Opening Completeness:** Given $$(j,d,op)$$, the data at location $j$ matches $d$ and after recomputing the root node using $op$ we get the same output.
- **Collision Resistance:** There cannot be two different openings to the same data at the same location.

## Using SHA256 in Ubuntu
Install libssl-dev and test by doing the following

```bash
# Install libssl-dev
sudo apt install libssl-dev

# Change permissions for run.sh
chmod +x run.sh

# Run the script
./run.sh
