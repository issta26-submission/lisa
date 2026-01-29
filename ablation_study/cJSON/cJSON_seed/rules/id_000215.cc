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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON *name = cJSON_CreateString("example");
    char buffer[256];
    int buf_len = (int)sizeof(buffer);
    memset(buffer, 0, buf_len);
    double set_ret = 0.0;
    double read_value = 0.0;
    const char *read_name = NULL;
    cJSON_bool printed_ok = 0;
    char first_char = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate / Validate
    set_ret = cJSON_SetNumberHelper(num, 42.5);
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    read_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    read_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    first_char = buffer[0];
    (void)set_ret;
    (void)read_value;
    (void)read_name;
    (void)printed_ok;
    (void)first_char;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}