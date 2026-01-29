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
//<ID> 858
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *detached = NULL;
    cJSON *dup_detached = NULL;
    char *printed = NULL;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool has_name = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    item0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, item0);
    item1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, item1);
    has_name = cJSON_HasObjectItem(root, "items");
    buffer[0] = has_name ? '1' : '0';

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 1);
    dup_detached = cJSON_Duplicate(detached, 1);
    equal = cJSON_Compare(detached, dup_detached, 1);
    printed = cJSON_PrintUnformatted(root);
    buffer[1] = equal ? '1' : '0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(dup_detached);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)buffer;
    (void)has_name;
    (void)equal;
    return 66;
}