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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.5);
    cJSON *str = cJSON_CreateString("initial");
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    double set_result = 0.0;
    double value_before = 0.0;
    double value_after = 0.0;
    char *printed = NULL;
    cJSON *dupe = NULL;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "text", str);
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddStringToObject(root, "note", "created for test");

    // step 3: Operate / Validate
    value_before = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    set_result = cJSON_SetNumberHelper(num, 3.1415);
    value_after = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    printed = cJSON_PrintUnformatted(root);
    ((char *)buffer)[0] = printed ? printed[0] : '\0';
    dupe = cJSON_Duplicate(root, 1);
    cmp = cJSON_Compare(root, dupe, 1);
    (void)set_result;
    (void)value_before;
    (void)value_after;
    (void)cmp;

    // step 4: Cleanup
    cJSON_Delete(dupe);
    cJSON_Delete(root);
    cJSON_free(printed);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}