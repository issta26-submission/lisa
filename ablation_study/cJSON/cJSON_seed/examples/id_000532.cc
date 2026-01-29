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
//<ID> 532
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *ver = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddStringToObject(root, "title", "example"); // uses required API
    cJSON_AddNullToObject(root, "optional"); // uses required API
    cJSON *s0 = cJSON_CreateString("one");
    cJSON *s1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, s0); // uses required API
    cJSON_AddItemToArray(items, s1); // uses required API

    // step 3: Operate & Validate
    char *snap1 = cJSON_PrintUnformatted(root);
    cJSON *parsed1 = cJSON_Parse(snap1);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed1, "items");
    cJSON_bool is_arr = cJSON_IsArray(parsed_items); // uses required API
    cJSON_DeleteItemFromArray(items, 1); // uses required API to remove the second item
    char *snap2 = cJSON_PrintUnformatted(root);
    cJSON *parsed2 = cJSON_Parse(snap2);
    cJSON_bool equal = cJSON_Compare(parsed1, parsed2, 1);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;is_array=%d;equal=%d",
            ver ? ver : "unknown",
            (int)is_arr,
            (int)equal);

    // step 4: Cleanup
    cJSON_free(snap1);
    cJSON_free(snap2);
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}