/**
 Copyright 2016 Otavio Rodolfo Piske

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <base/common/bmic_object.h>
#include <base/format/bmic_json.h>

#include <io/gru_ioutils.h>

#include <context/bmic_context.h>

int main(int argc, char **argv) {
	char *buff = NULL;

	gru_status_t status = {0};
	bmic_log_initialization();

	FILE *file = gru_io_open_file_read_path(argv[1], &status);

	if (!file || status.code != GRU_SUCCESS) {
		fprintf(stderr, "%s", status.message);
		return EXIT_FAILURE;
	}

	size_t size = gru_io_read_text_into(&buff, file, &status);
	if (size == 0) {
		fprintf(stderr, "%s", status.message);
		fclose(file);

		return EXIT_FAILURE;
	}
	fclose(file);

	printf("Read %lu bytes from the file\n", size);

	bmic_json_t *json = bmic_json_init(buff, &status);
	if (json == NULL) {
		goto err_exit;
	}
	gru_dealloc_string(&buff);

	bmic_object_t *root = bmic_object_new_root(&status);
	if (!root) {
		goto err_exit;
	}

	bmic_json_transform(json, root);

	const bmic_object_t *type = bmic_object_find_by_name(root, "status");
	if (type == NULL) {
		bmic_object_destroy(&root);
		fprintf(stderr, "Child not found\n");

		goto err_exit;
	}

	const bmic_object_t *name1 = bmic_object_find_by_path(root, "/args/args[1]/name");
	if (name1 == NULL) {
		bmic_object_destroy(&root);
		fprintf(stderr, "Child not found\n");

		goto err_exit;
	}

	if (name1->type != BMIC_STRING) {
		fprintf(stderr, "Invalid node type returned\n");

		bmic_object_destroy(&root);
		goto err_exit;
	}
	printf("Node found: %s [%s]\n", name1->name, name1->path);

	if (strcmp(name1->data.str, "maximumMessages") != 0) {
		fprintf(stderr, "The node data is not the expected value: %s != %s\n",
			name1->data.str, "maximumMessages");

		bmic_object_destroy(&root);
		goto err_exit;
	}

	const bmic_object_t *complex = bmic_object_find_by_path(root, "/complex");
	if (complex == NULL || strcmp(complex->name, "complex") != 0) {
		fprintf(stderr, "Complex node not found\n");

		bmic_object_destroy(&root);
		goto err_exit;
	}
	printf("Node found: %s [%s]\n", complex->name, complex->path);

	const bmic_object_t *child3 = bmic_object_find_child_by_name(complex, "child3");
	if (child3 == NULL || (strcmp(child3->name, "child3") != 0)) {
		fprintf(stderr, "Complex node child 'child3' not found\n");

		bmic_object_destroy(&root);
		goto err_exit;
	}
	printf("Node found: %s [%s]\n", child3->name, child3->path);

	bmic_json_destroy(&json);
	bmic_object_destroy(&root);
	return EXIT_SUCCESS;

err_exit:

	if (status.code != GRU_SUCCESS) {
		fprintf(stderr, "%s", status.message);
	}

	gru_dealloc_string(&buff);
	gru_status_reset(&status);
	bmic_json_destroy(&json);

	return EXIT_FAILURE;
}