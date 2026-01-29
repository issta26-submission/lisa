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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14159265);
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *parsed = NULL;
    cJSON *parsed_addnum = NULL;
    cJSON *parsed_child = NULL;
    cJSON *parsed_created_num = NULL;
    double val_addnum = 0.0;
    double val_created = 0.0;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "created_num", num_item);
    cJSON_AddNumberToObject(root, "addnum", 2.718281828);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    parsed = cJSON_ParseWithLength(printed, printed_len);
    parsed_addnum = cJSON_GetObjectItem(parsed, "addnum");
    val_addnum = cJSON_GetNumberValue(parsed_addnum);
    parsed_child = cJSON_GetObjectItem(parsed, "child");
    parsed_created_num = cJSON_GetObjectItem(parsed_child, "created_num");
    val_created = cJSON_GetNumberValue(parsed_created_num);
    buffer[0] = (char)('0' + ((int)val_addnum % 10));
    buffer[1] = (char)('A' + ((int)val_created % 26));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}