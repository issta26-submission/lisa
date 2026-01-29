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
//<ID> 1631
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON_bool has_child = 0;
    cJSON_bool printed = 0;
    char *buffer = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    cJSON_AddNullToObject(child, "maybe");
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemReferenceToObject(root, "number_ref", num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemReferenceToObject(child, "greeting_ref", str);

    // step 3: Operate / Validate
    has_child = cJSON_HasObjectItem(root, "child");
    buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    printed = cJSON_PrintPreallocated(root, buffer, 256, has_child);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}