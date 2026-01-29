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
//<ID> 1404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *elem1 = NULL;
    cJSON *elem2 = NULL;
    cJSON *ref_item = NULL;
    char *buffer = NULL;
    const int buf_len = 256;
    cJSON_bool print_ok = 0;
    cJSON_bool add_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem1 = cJSON_CreateString("elem1");
    elem2 = cJSON_CreateString("elem2");
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddStringToObject(root, "greeting", "hello");
    ref_item = cJSON_CreateObject();
    cJSON_AddStringToObject(ref_item, "inner", "x");
    cJSON_AddItemReferenceToObject(root, "inner_ref", ref_item);

    // step 3: Operate / Validate
    buffer = (char *)cJSON_malloc((size_t)buf_len);
    print_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    add_ok = cJSON_AddItemToArray(arr, cJSON_CreateString("elem3"));
    (void)print_ok;
    (void)add_ok;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}