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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *label = cJSON_CreateString("example");
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    double before = cJSON_GetNumberValue(num);
    double new_value = 3.14159;
    double set_ret = 0.0;
    double after = 0.0;
    double delta = 0.0;
    int idelta = 0;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    set_ret = cJSON_SetNumberHelper(num, new_value);
    after = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    delta = after - before;
    idelta = (int)(delta * 100.0);
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    buffer[0] = ver[0];
    buffer[1] = (char)(printed_ok + '0');
    buffer[2] = (char)(idelta & 0xFF);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}