#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 2066
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    char *unfmt = NULL;
    char *bufed = NULL;
    double got_value = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();

    // step 3: Configure
    num_item = cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 4: Operate
    unfmt = cJSON_PrintUnformatted(root);
    bufed = cJSON_PrintBuffered(root, 64, 0);
    got_value = cJSON_GetNumberValue(num_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(unfmt != NULL);
    validation ^= (int)(bufed != NULL);
    validation ^= (int)(got_value == 3.1415);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(unfmt);
    cJSON_free(bufed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}