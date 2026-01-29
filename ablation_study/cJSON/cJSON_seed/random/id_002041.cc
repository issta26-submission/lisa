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
//<ID> 2041
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
    cJSON *true_item = NULL;
    cJSON *added_item = NULL;
    char *strval = NULL;
    void *mem = NULL;
    cJSON_bool invalid_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    added_item = cJSON_AddBoolToObject(root, "active", 1);
    mem = cJSON_malloc(16);
    memset((char *)mem, 'X', 15);
    if (mem) ((char *)mem)[15] = '\0';

    // step 4: Operate
    strval = cJSON_GetStringValue(added_item);
    invalid_flag = cJSON_IsInvalid(true_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(added_item != NULL);
    validation ^= (int)(mem != NULL);
    validation ^= (int)(strval == NULL);
    validation ^= (int)(invalid_flag == 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(mem);
    cJSON_Delete(true_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}