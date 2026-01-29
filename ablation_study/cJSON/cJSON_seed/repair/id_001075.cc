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
//<ID> 1075
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *it1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, it1);
    cJSON *it2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, it2);

    // step 2: Configure
    cJSON *external = cJSON_Parse("\"external-value\"");
    cJSON_AddItemReferenceToObject(root, "external", external);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    int count = cJSON_GetArraySize(items);
    char *extstr = cJSON_GetStringValue(external);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = extstr[0];
    ((char *)scratch)[1] = (char)('0' + (count & 0xF));
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}