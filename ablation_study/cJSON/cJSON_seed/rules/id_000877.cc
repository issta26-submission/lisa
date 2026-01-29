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
//<ID> 877
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *retrieved = NULL;
    cJSON *num_clone = NULL;
    char *buffer = NULL;
    double value = 0.0;
    cJSON_bool add_res1 = 0;
    cJSON_bool add_res2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);
    add_res1 = cJSON_AddItemToObject(root, "original", num_item);
    buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "original");
    value = cJSON_GetNumberValue(retrieved);
    num_clone = cJSON_CreateNumber(value * 2.0);
    add_res2 = cJSON_AddItemToObject(root, "double", num_clone);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "original");
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)add_res1;
    (void)add_res2;
    (void)value;
    (void)retrieved;
    return 66;
}