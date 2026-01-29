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
//<ID> 1224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_item);

    // step 2: Configure
    char *buf = (char *)cJSON_malloc((size_t)64);
    memset(buf, 0, (size_t)64);
    buf[0] = '[';
    buf[1] = ' ';
    buf[2] = '"';
    buf[3] = 'o';
    buf[4] = 'n';
    buf[5] = 'e';
    buf[6] = '"';
    buf[7] = ' ';
    buf[8] = ',';
    buf[9] = ' ';
    buf[10] = '"';
    buf[11] = 't';
    buf[12] = 'w';
    buf[13] = 'o';
    buf[14] = '"';
    buf[15] = ' ';
    buf[16] = ']';
    cJSON_Minify(buf);
    cJSON *parsed = cJSON_Parse(buf);
    cJSON_AddItemToObject(root, "list", parsed);
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate
    cJSON_bool is_array_flag = cJSON_IsArray(parsed);
    cJSON_AddBoolToObject(root, "parsed_is_array", is_array_flag);
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(parsed);

    // step 4: Validate & Cleanup
    (void)arr_size;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}