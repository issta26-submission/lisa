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
//<ID> 209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON *label = cJSON_CreateString("initial");
    cJSON *flag_node = NULL;
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *ver = cJSON_Version();
    ((char *)buffer)[0] = ver[0];
    double before = cJSON_GetNumberValue(num);
    double new_value = 3.14159;
    double set_ret = 0.0;
    double after = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "label", label);
    flag_node = cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate / Validate
    before = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    set_ret = cJSON_SetNumberHelper(num, new_value);
    after = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    printed = cJSON_PrintUnformatted(root);
    ((char *)buffer)[1] = printed[0];
    (void)before;
    (void)set_ret;
    (void)after;
    (void)flag_node;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}