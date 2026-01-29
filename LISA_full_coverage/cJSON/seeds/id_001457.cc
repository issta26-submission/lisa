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
//<ID> 1457
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    const char json_text[] = "{\"numbers\":[1,2,3],\"meta\":{\"val\":10}}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");

    cJSON *new_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(numbers, new_num);
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    /* Use a duplicate of the numbers array instead of a reference to avoid
       shared ownership / double-free issues when deleting the root. */
    cJSON *array_ref = cJSON_Duplicate(numbers, 1);

    cJSON_AddItemToObject(root, "numbers_ref", array_ref);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    char *printed = cJSON_PrintUnformatted(root);
    int numbers_size = cJSON_GetArraySize(numbers);
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "numbers_ref");
    (void)numbers_size;
    (void)first_val;
    (void)has_ref;

    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(meta_dup);
    cJSON_Delete(root);
    return 66;
}