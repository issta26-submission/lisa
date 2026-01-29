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
//<ID> 1915
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *label_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label_item);
    cJSON *count_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *detail = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "detail", detail);
    cJSON *nested_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(detail, "pi", nested_num);

    // step 2: Configure
    cJSON *new_count = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "count", new_count);
    char *setres = cJSON_SetValuestring(label_item, "updated");
    cJSON *added = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(detail, "lucky", added);

    // step 3: Operate and Validate
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    char *lbl = cJSON_GetStringValue(label_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = setres ? setres[0] : 'Y';
    buffer[2] = (char)('0' + ((int)count_val % 10));
    buffer[3] = lbl ? lbl[0] : 'Z';
    buffer[4] = (char)('0' + (replaced % 10));
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}