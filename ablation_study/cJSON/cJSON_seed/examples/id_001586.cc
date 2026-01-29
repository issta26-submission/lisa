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
//<ID> 1586
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names_arr[] = {"alice", "bob", "carol"};
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);

    // step 2: Configure
    cJSON *ref_item = cJSON_CreateString("dave");
    cJSON_AddItemReferenceToArray(names, ref_item);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromArray(names, 1);
    cJSON_bool has = cJSON_HasObjectItem(root, "names");
    cJSON_AddNumberToObject(root, "has_names", (double)has);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}