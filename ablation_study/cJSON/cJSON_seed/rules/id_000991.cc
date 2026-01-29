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
//<ID> 991
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *data_array = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *num_insert = NULL;
    char *buffered_print = NULL;
    int prebuffer = 128;
    cJSON_bool pretty = 1;
    cJSON_bool is_array = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    data_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data_array);
    num0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(data_array, num0);
    num1 = cJSON_CreateNumber(30.5);
    cJSON_AddItemToArray(data_array, num1);
    num_insert = cJSON_CreateNumber(20.5);
    cJSON_InsertItemInArray(data_array, 1, num_insert);

    // step 3: Operate / Validate
    is_array = cJSON_IsArray(data_array);
    buffered_print = cJSON_PrintBuffered(root, prebuffer, pretty);

    // step 4: Cleanup
    cJSON_free(buffered_print);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}