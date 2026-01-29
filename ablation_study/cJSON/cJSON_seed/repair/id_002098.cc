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
//<ID> 2098
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *s_one = cJSON_CreateString("one");
    cJSON *s_two = cJSON_CreateString("two");
    cJSON *s_three = cJSON_CreateString("three");
    cJSON_AddItemToObject(root, "items", items);
    cJSON_InsertItemInArray(items, 0, s_two);
    cJSON_InsertItemInArray(items, 1, s_three);
    cJSON_InsertItemInArray(items, 1, s_one);
    cJSON_AddStringToObject(root, "category", "numbers");

    // step 2: Configure
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);

    // step 3: Operate and Validate
    cJSON *dump_str = cJSON_CreateString(buffer);
    cJSON_AddStringToObject(root, "dump", buffer);
    cJSON_InsertItemInArray(items, 3, dump_str);
    (void)printed_ok; // preserve printed_ok usage without branching

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}