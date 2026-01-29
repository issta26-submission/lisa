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
//<ID> 993
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *n_insert = NULL;
    char *printed = NULL;
    char *preallocated = NULL;
    int pre_len = 256;
    int prebuffer = 128;
    cJSON_bool fmt = 1;
    cJSON_bool is_arr = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToObject(root, "source", cJSON_CreateString("unitX"));
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(30.0);
    n_insert = cJSON_CreateNumber(20.0);

    // step 3: Operate / Validate
    cJSON_InsertItemInArray(values, 0, n0);
    cJSON_InsertItemInArray(values, 1, n1);
    cJSON_InsertItemInArray(values, 1, n_insert);
    is_arr = cJSON_IsArray(values);
    printed = cJSON_PrintBuffered(root, prebuffer, fmt);
    preallocated = (char *)cJSON_malloc((size_t)pre_len);
    memset(preallocated, 0, (size_t)pre_len);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(preallocated);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}