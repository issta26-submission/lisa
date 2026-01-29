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
//<ID> 218
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
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    double new_value = 42.5;
    double set_ret = 0.0;
    cJSON_bool print_ok = 0;
    cJSON *retrieved = NULL;
    double read_back = 0.0;
    int combined_int = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num);

    // step 3: Operate / Validate
    set_ret = cJSON_SetNumberHelper(num, new_value);
    print_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    retrieved = cJSON_GetObjectItem(root, "value");
    read_back = cJSON_GetNumberValue(retrieved);
    combined_int = (int)(set_ret + read_back);
    buffer[0] = (char)(combined_int & 0xFF);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}