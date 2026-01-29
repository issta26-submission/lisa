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
//<ID> 555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"obj\":{\"name\":\"Charlie\"}}";
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_Parse(json);

    // step 2: Setup / Configure
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *name_item = cJSON_GetObjectItem(obj, "name");
    cJSON *added_num = cJSON_AddNumberToObject(root, "version", 1.23);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag);
    cJSON *name_ref = cJSON_CreateStringReference("CharlieRef");
    cJSON_AddItemToObject(obj, "ref", name_ref);

    // step 3: Operate / Validate
    char *ref_str = cJSON_GetStringValue(name_ref);
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver_val = cJSON_GetNumberValue(ver_item);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t copy_len = printed_len > 240 ? 240 : printed_len;
    memcpy(buffer, printed, copy_len);
    buffer[copy_len] = '\0';
    buffer[0] = ref_str ? ref_str[0] : '\0';
    (void)ver_val;
    (void)name_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}