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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child1 = cJSON_CreateObject();
    cJSON *num_node = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(child1, "value", num_node);
    cJSON *child2 = cJSON_Duplicate(child1, 1);
    cJSON_AddItemToObject(root, "alpha", child1);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemViaPointer(root, child1);
    cJSON_bool has_alpha = cJSON_HasObjectItem(root, "alpha");
    cJSON_bool are_equal = cJSON_Compare(detached, child2, 1);

    // step 3: Operate and Validate
    cJSON_AddNumberToObject(root, "has_alpha", (double)has_alpha);
    cJSON_AddNumberToObject(root, "are_equal", (double)are_equal);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = (int)has_alpha + (int)are_equal + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    cJSON_Delete(child2);

    // API sequence test completed successfully
    return 66;
}