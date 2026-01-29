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
//<ID> 2067
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
    char *unformatted = NULL;
    char *buffered = NULL;
    double gotnum = 0.0;
    int validation = 0;
    cJSON_bool fmt = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "value", 42.5);

    // step 3: Core operations
    unformatted = cJSON_PrintUnformatted(root);
    buffered = cJSON_PrintBuffered(root, 128, fmt);
    gotnum = cJSON_GetNumberValue(num_item);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(buffered != NULL);
    validation ^= (int)(gotnum == 42.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_free(buffered);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}